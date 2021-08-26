#ifndef __COORDINATESYSTEM_H__
#define __COORDINATESYSTEM_H__

#include "vector.h"

class CoordinateSystem
{
	public:
		CoordinateSystem();
		virtual ~CoordinateSystem();

		vector x1();
		vector y1();
		vector z1();

		void rotate( vector &v, float fi );

	private:
		vector m_x1;
		vector m_y1;
		vector m_z1;
};

#endif