#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <d3d8.h>
#include <d3dx8math.h>

#pragma comment ( lib, "d3d8.lib" )
#pragma comment ( lib, "d3dx8.lib" )


class matrix
{
	public:
		matrix();
		matrix( int rows, int cols );
		virtual ~matrix();

		int rows();
		int cols();

		float operator() ( int row, int col );
		float* operator() ( int row );

		void set( int row, int col, float f );

		float** array();
		D3DXMATRIX elements();

		matrix operator+ ( matrix &m );
		matrix operator- ( matrix &m );
		matrix operator* ( float t );
		matrix operator/ ( float t );
		matrix operator* ( matrix &m );

		matrix invert();
		matrix transpose();

	protected:
		int m_rows;
		int m_cols;

		float **m_array;
		D3DXMATRIX m_elements;
};

#endif
