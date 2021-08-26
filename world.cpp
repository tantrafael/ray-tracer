#include "World.h"
#include "Game.h"

World::World()
{
	m_SkyColor  = Color(0,0,0);
	m_pGame	    = NULL;
	m_pCamera   = new Camera( vector(0,0,0), 10.0f );
	m_pObjects  = NULL;
	m_pLights   = NULL;
}

World::~World()
{
//	delete m_pSurface;
//	delete m_pCamera;
}

void World::init( Game *pGame )
{
	m_pGame = pGame;

	m_pCamera->setPos( vector( -300.0f, 0.0f, 250.0f ) );
//	m_pCamera->rotate( m_pCamera->getCoordinateSystem()->z1(), 0.125f*pi );
	m_pCamera->rotate( m_pCamera->getCoordinateSystem()->y1(), 0.25f*pi );

	m_nrObjects = 3;
	m_pObjects = new Sphere[ m_nrObjects ];

	for( int i=0; i<m_nrObjects; i++ )
	{
		float k = i / float( m_nrObjects );
		float t = 2*pi * k + 0.1f*pi;
		vector pos = vector( cosf(t), sinf(t), 0.0f ) * 150.0f;

	//	float R = 50.0f + 25.0f * i;
		float R = 100.0f;

		Color c;

		switch( i % 3 )
		{
			case 0: c = Color(0,255,0); break;
			case 1: c = Color(255,0,0); break;
			case 2: c = Color(0,0,255); break;
		}

		float r = 0.75f;

		m_pObjects[i] = Sphere( pos, R, c, r );
	}


	m_nrLights = 1;
	m_pLights = new Light[ m_nrLights ];

	for( int i=0; i<m_nrLights; i++ )
	{
		float k = i / float( m_nrLights );
		float t = 2*pi * k;
		vector pos = vector( 0.0f, -1.0f, 1.25f ) * 250.0f;

		m_pLights[i] = Light( pos, Color(255,255,255) );
	}

	m_pGame->graphics()->ClearBuffer( &m_SkyColor );
	m_pGame->graphics()->Present();
}

//-----------------------------------------------------------------------------
// Access
//-----------------------------------------------------------------------------
Camera*  World::camera()   { return m_pCamera; }
Color    World::SkyColor() { return m_SkyColor; }


//-----------------------------------------------------------------------------
// update()
//-----------------------------------------------------------------------------
void World::update()
{
	m_UpdateCamera();
}


//-----------------------------------------------------------------------------
// draw()
//-----------------------------------------------------------------------------
void World::draw()
{
	m_pGame->graphics()->ClearBuffer( &m_SkyColor );
	m_pGame->graphics()->BeginScene();

	RayTrace( m_pObjects, m_pLights );

	m_pGame->graphics()->EndScene();
	m_pGame->graphics()->Present();
}


//-----------------------------------------------------------------------------
// RayTrace()
//-----------------------------------------------------------------------------
void World::RayTrace( Sphere *pObjects, Light *pLights )
{
	vector p0 = m_pCamera->pos();

	int rows = m_pGame->window()->height();
	int cols = m_pGame->window()->width();

	for( int row = 0; row < rows; row++ )
	{
		for( int col = 0; col < cols; col++ )
		{
			float  x  = m_pCamera->R();
			float  y  = float(  col - cols/2 );
			float  z  = float( -row + rows/2 );
			vector v0 = m_pCamera->translate( normalize( vector(x,y,z) ) );

			if( Intersects( p0, v0, pObjects ) )
			{
				Color c = Trace( p0, v0, Color(16,16,16), pObjects, pLights, 1.0f );
				m_pGame->graphics()->PutPixel( col, row, clamp(c) );
			}
		}
	}
}


//-----------------------------------------------------------------------------
// Trace()
//-----------------------------------------------------------------------------
Color World::Trace( vector &p0, vector &v0, Color &c0,
				    Sphere *pObjects, Light *pLights,
					float intensity0 )
{
	Color c = c0;

	if( Intersects( p0, v0, pObjects ) )
	{
		Intersection *pIntersection = GetIntersection( p0, v0, pObjects );
		vector  p1 = pIntersection->pos;
		Sphere *s  = &( pIntersection->obj );
		vector  n  = normalize( p1 - s->pos() );

		c = c0 + GetColor( p1, n, s, pLights ) * intensity0;

		float intensity1 = intensity0 * s->reflexivity();

		if( intensity1 > 0.001f )
		{
			vector v1 = Reflection( v0, n );
		//	vector v1 = n * (v0*n*2) - v0;
			c = Trace( p1, v1, c, pObjects, pLights, intensity1 );
		//	vector v2 = vector(0,0,1);
		//	c = Trace( p1, v2, c, pObjects, pLights, iteration + 1 );
		}
	}

	return c;
}


vector World::Reflection( vector &v, vector &n )
{
	return v - n * (v*n*2);
}


//-----------------------------------------------------------------------------
// Intersects()
//-----------------------------------------------------------------------------
bool World::Intersects( vector &p, vector &v, Sphere *pObjects )
{
	bool intersected = false;

	int i = 0;

	while( i < m_nrObjects && !intersected )
	{
		vector d  = pObjects[i].pos() - p;
		float  r  = pObjects[i].R();

		float vd = v * d;
		float d2 = d.x()*d.x() + d.y()*d.y() + d.z()*d.z();
		float B  = vd*vd + r*r - d2;

		if( B >= 0 )
		{
			float b = sqrtf( B );
			if( vd + b > 1.0f || vd - b > 1.0f )
			{
				intersected = true;
			}
		}

		i++;
	}

	return intersected;
}


//-----------------------------------------------------------------------------
// GetIntersection()
//-----------------------------------------------------------------------------
Intersection* World::GetIntersection( vector &p, vector &v, Sphere *pObjects )
{
	Intersection *pIntersection = new Intersection;

	float t_min = 1000000.0f;
	int   i_min = 0;

	for( int i=0; i<m_nrObjects; i++ )
	{
		float t = 0;

		vector d  = pObjects[i].pos() - p;
		float  r  = pObjects[i].R();

		float vd = v * d;
		float d2 = d.x()*d.x() + d.y()*d.y() + d.z()*d.z();

		float B  = vd*vd + r*r - d2;
		if( B >= 0 )
		{
			float b  = sqrtf( B );
			float t1 = vd + b;
			float t2 = vd - b;

			if( t1 > 1.0f )
			{
				t = t1;
				if( 0 < t2 && t2 < t1 ) { t = t2; }
			}

			if( t2 > 1.0f )
			{
				t = t2;
				if( 0 < t1 && t1 < t2 ) { t = t1; }
			}
		}

		if( 0 < t && t < t_min )
		{
			t_min = t;
			i_min = i;
		}
	}

	pIntersection->pos = p + v * t_min;
	pIntersection->obj = pObjects[ i_min ];

	return pIntersection;
}


//-----------------------------------------------------------------------------
// GetColor()
//-----------------------------------------------------------------------------
Color World::GetColor( vector p, vector n, Sphere *s, Light *pLights )
{
	float k1 = 0;
	float k2 = 0;

	for( int i=0; i<m_nrLights; i++ )
	{
		vector L = normalize( pLights[i].pos() - p );
		vector R = Reflection( L, n );
		vector E = normalize( p - m_pCamera->pos() );

		float t1 = L * n;
		if( t1 < 0 ) { t1 = 0; }
		k1 += t1;

		float t2 = R * E;
		if( t2 < 0 ) { t2 = 0; }
		k2 += powf( t2, 25.0f );
	}

	if( k1 > 1.0f ) { k1 = 1.0f; }
	if( k2 > 1.0f ) { k2 = 1.0f; }

	return s->color() * k1 + Color(255,255,255) * k2;
}


//-----------------------------------------------------------------------------
// m_UpdateCamera()
//-----------------------------------------------------------------------------
void World::m_UpdateCamera()
{
//	float t = 0.001f * GetTickCount();
//	m_pCamera->rotate( m_pCamera->getCoordinateSystem()->x1(), 0.025f );
	m_pCamera->update();
}
