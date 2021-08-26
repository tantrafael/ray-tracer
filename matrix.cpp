#include "matrix.h"

//--------------------------------------------------------------------------
// Constructors, destructors
//--------------------------------------------------------------------------

matrix::matrix()
{
	m_rows = 3;
	m_cols = 3;

	m_array = new float*[m_rows];

	for( int i=0; i<m_rows; i++ )
	{
		m_array[i] = new float[m_cols];
	}
}

matrix::matrix( int rows, int cols )
{
	m_rows = rows;
	m_cols = cols;

	m_array = new float*[m_rows];

	for( int i=0; i<m_rows; i++ )
	{
		m_array[i] = new float[m_cols];
	}
}

matrix::~matrix()
{
	for( int i=0; i<m_rows; i++ )
	{
		delete m_array[i];
	}

	delete m_array;
}


//--------------------------------------------------------------------------
// Access functions
//--------------------------------------------------------------------------

int matrix::rows() { return m_rows; }
int matrix::cols() { return m_cols; }


float matrix::operator() ( int row, int col )
{
	return m_array[row][col];
}


float* matrix::operator() ( int row )
{
	return m_array[row];
}


void matrix::set( int row, int col, float f )
{
	m_array[row][col] = f;
}


float** matrix::array()
{
	return m_array;
}

D3DXMATRIX matrix::elements()
{
	return m_elements;
}


//--------------------------------------------------------------------------
// Operators
//--------------------------------------------------------------------------

matrix matrix::operator+ ( matrix &m )
{
	matrix A;

	for( int row=0; row<m_rows; row++ )
	{
		for( int col=0; col<m_cols; col++ )
		{
			A.set( row, col, (row,col) + m(row,col) );
		}
	}

	return A;
}

matrix matrix::operator- ( matrix &m )
{
	matrix A;

	for( int row=0; row<m_rows; row++ )
	{
		for( int col=0; col<m_cols; col++ )
		{
			A.set( row, col, (row,col) - m(row,col) );
		}
	}

	return A;
}

matrix matrix::operator* ( float t )	// skalärprodukt
{
	matrix A;

	for( int row=0; row<m_rows; row++ )
	{
		for( int col=0; col<m_cols; col++ )
		{
			A.set( row, col, (row,col) * t );
		}
	}

	return A;
}

matrix matrix::operator/ ( float t )
{
	matrix A;

	for( int row=0; row<m_rows; row++ )
	{
		for( int col=0; col<m_cols; col++ )
		{
			A.set( row, col, (row,col) / t );
		}
	}

	return A;
}

matrix matrix::operator* ( matrix &m )	// matrisprodukt
{
	matrix A;

	for( int row=0; row<m_rows; row++ )
	{
		for( int col=0; col<m.cols(); col++ )
		{
			float f = 0;

			for( int i=0; i<m_cols; i++ )
			{
				f += (row,i) * m(i,col);
			}

			A.set( row, col, f );
		}
	}

	return A;
}


//--------------------------------------------------------------------------
// Functions
//--------------------------------------------------------------------------

matrix matrix::invert()
{
	matrix A;

	D3DXMATRIX copy;

	for( int row=0; row<3; row++ )
	{
		for( int col=0; col<3; col++ )
		{
			copy(row,col) = m_array[row][col];
		}
	}

	copy(3,0) = 0.0f;
	copy(3,1) = 0.0f;
	copy(3,2) = 0.0f;

	copy(0,3) = 0.0f;
	copy(1,3) = 0.0f;
	copy(2,3) = 0.0f;

	copy(3,3) = 1.0f;

	D3DXMATRIX result;

	D3DXMatrixInverse( &result, NULL, &copy );

	for( int row=0; row<3; row++ )
	{
		for( int col=0; col<3; col++ )
		{
			A.set( row, col, result(row,col) );
		}
	}

	return A;
}

matrix matrix::transpose()
{
	return *this;
}
