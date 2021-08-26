#include "vector.h"

//--------------------------------------------------------------------------
// Constructors, destructors
//--------------------------------------------------------------------------

vector::vector()
{
	m_x = 0;
	m_y = 0;
	m_z = 0;
}

vector::vector( float x, float y, float z )
{
	m_x = x;
	m_y = y;
	m_z = z;
}

vector::~vector() {}


//--------------------------------------------------------------------------
// Rotation
//--------------------------------------------------------------------------

float vector::x()	{ return m_x; }
float vector::y()	{ return m_y; }
float vector::z()	{ return m_z; }


//--------------------------------------------------------------------------
// Operators
//--------------------------------------------------------------------------

vector vector::operator+ ( vector &v )
{	return vector( x()+v.x(), y()+v.y(), z()+v.z() );	}

vector vector::operator- ( vector &v )
{	return vector( x()-v.x(), y()-v.y(), z()-v.z() );	}

vector vector::operator* ( float t )
{	return vector( x()*t, y()*t, z()*t );	}

vector vector::operator/ ( float t )
{	return vector( x()/t, y()/t, z()/t );	}

float vector::operator* ( vector &v )	// skal�rprodukt
{	return ( x()*v.x() + y()*v.y() + z()*v.z() );	}

vector vector::operator^ ( vector &v )	// vektorprodukt
{
	return vector(	y()*v.z() - z()*v.y(),
					z()*v.x() - x()*v.z(),
					x()*v.y() - y()*v.x() );
}


//--------------------------------------------------------------------------
// Rotation
//--------------------------------------------------------------------------

void vector::rotate( vector &v, float fi )	// kring vektor v, med vinkel fi.
{
										// L�t rotationsaxeln utg�ra z-axeln i ett
										// t�nkt koordinatsystem, med enhetsvektorer x1, y1, z1.

	if( norm( v ) > 0.001f )			// Bara rotation kring en vektor
										// skild fr�n 0 ger resultat.
	{
		vector z1 = normalize( v );		// Vektorn v med rotationsaxelns riktning ger z1
										// genom normering.

		float s = norm( z1 ^ *this );	// Ber�kna l�ngden av projektionen
										// p� normalplanet till rotationsaxeln.
										// Den ska bli v�rt x1-v�rde.

										// Skulle detta vara 0,
										// d.v.s. vektorn som ska roteras
										// sammanfaller med rotationsaxeln
										// p�verkas den ej.

		if( s > 0.001f )				// I annat fall �r s > 0, men vi skriver
		{								// 0.0001 f�r s�kerhets skull
										// s� att inga avrundningar ska kunna
										// orsaka division med 0 vid normering.

			float t = z1 * (*this);		// Ber�kna l�ngden av projektionen
										// p� rotationsaxeln. Den blir v�r z1-koordinat.

			vector w = *this - z1*t;	// L�t projektionen p� normalplanet till rotationsaxeln
										// ange riktningen f�r v�r t�nkta x-axel.
			vector x1 = normalize(w);	// Normering ger x1.
									
			vector y1 = x1 ^ z1;		// y1 �r vinkelr�t mot x1 och z1,
										// och ges av deras vektorprodukt.

			*this = x1*s*cosf(fi) +		// I v�rt t�nkta koordinatsystem blir
					y1*s*sinf(fi) +		// rotationen kring z-axeln
					z1*t;				// nu en enkel ber�kning.
		}
	}
}


vector rotation( vector &pos, vector &axis, float fi )
{
	vector r = pos;
	r.rotate( axis, fi );
	return r;
}


//--------------------------------------------------------------------------
// Normation
//--------------------------------------------------------------------------

float norm( vector &v )
{	return sqrtf( v.x()*v.x() + v.y()*v.y() + v.z()*v.z() );	}


vector normalize( vector &v )
{
	float t = norm( v );
	if( t > 0.0f )
		return v / t;
	else
		return vector(0,0,0);
}
