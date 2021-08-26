#ifndef __WORLD_H__
#define __WORLD_H__

#include "constants.h"
#include "Camera.h"
#include "Light.h"
#include "Sphere.h"

class Game;


struct Intersection
{
	vector pos;
	Sphere obj;
};


class World
{
	public:
		World();
		virtual ~World();

		void init( Game *pGame );
		Camera* camera();
		Color SkyColor();
		void update();
		void draw();

	protected:
		Color   m_SkyColor;
		Game   *m_pGame;
		Camera *m_pCamera;

		int m_nrObjects;
		Sphere *m_pObjects;

		int m_nrLights;
		Light *m_pLights;

		void m_UpdateCamera();

		void RayTrace( Sphere *pObjects, Light *pLights );

		Color Trace( vector &p0, vector &v0, Color &c0,
					 Sphere *Objects, Light *pLights,
					 float intensity0 );

		vector Reflection( vector &v, vector &n );

		bool Intersects( vector &p, vector &v, Sphere *pObjects );

		Intersection* GetIntersection( vector &p, vector &v, Sphere *pObjects );

		Color GetColor( vector p, vector n, Sphere *s, Light *pLights );
};

#endif