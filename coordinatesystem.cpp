#include <math.h>

#include "CoordinateSystem.h"

CoordinateSystem::CoordinateSystem()
{
	m_x1 = vector( 1.0f, 0, 0 );
	m_y1 = vector( 0, 1.0f, 0 );
	m_z1 = vector( 0, 0, 1.0f );
}

CoordinateSystem::~CoordinateSystem() {}

vector CoordinateSystem::x1() { return m_x1; }
vector CoordinateSystem::y1() { return m_y1; }
vector CoordinateSystem::z1() { return m_z1; }

void CoordinateSystem::rotate( vector &v, float fi )
{
	m_x1.rotate( v, fi );
	m_y1.rotate( v, fi );
	m_z1.rotate( v, fi );

	m_x1 = normalize( m_x1 );		// För att
	m_y1 = normalize( m_y1 );		// undvika
	m_z1 = normalize( m_z1 );		// avrundningsfel
}
