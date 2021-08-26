#include "Sphere.h"

Sphere::Sphere()
{}

Sphere::Sphere( vector &pos, float R, Color &color, float r ):Body( pos )
{
	m_pGame = NULL;
	m_R = R;
	m_color = color;
	m_reflexivity = r;
}

Sphere::~Sphere() {}


float Sphere::R()			{ return m_R; }
Color Sphere::color()		{ return m_color; }
float Sphere::reflexivity() { return m_reflexivity; }
