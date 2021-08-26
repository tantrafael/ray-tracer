#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "Body.h"
#include "Graphics.h"

class Game;


class Sphere : public Body
{
	public:
		Sphere();
		Sphere( vector &pos, float R, Color &color, float r );
		~Sphere();

		float R();
		Color color();
		float reflexivity();

	protected:
		Game *m_pGame;

		float m_R;
		Color m_color;
		float m_reflexivity;
};

#endif