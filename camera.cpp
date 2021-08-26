#include "Camera.h"
#include "Game.h"
#include "constants.h"

Camera::Camera()
{}

Camera::Camera( vector v ):Body(v)
{}

Camera::Camera( vector v, float m ):Body( v, m )
{
	m_pGame = NULL;

	m_R = 300.0f;
	m_P0.x = 0;
	m_P0.y = 0;
	m_ViewPort.left   = 0;
	m_ViewPort.top    = 0;
	m_ViewPort.right  = 0;
	m_ViewPort.bottom = 0;
}

Camera::~Camera() {}


void Camera::init( Game *pGame )
{
	m_pGame = pGame;
	m_P0.x = m_pGame->window()->width()  / 2;
	m_P0.y = m_pGame->window()->height() / 2;
}


float Camera::R() { return m_R; }
RECT* Camera::ViewPort() { return &m_ViewPort; }


POINT Camera::project( vector &pos )
{
	vector p = translate( pos - m_pos );
	float  q = m_R / p.x();
	float y1 = p.y() * q;
	float z1 = p.z() * q;

	POINT P;
	P.x = int( m_P0.x + y1 );
	P.y = int( m_P0.y - z1 );

	return P;
}


vector Camera::translate( vector &pos )
{
	vector x1 = m_pCoordinateSystem->x1();
	vector y1 = m_pCoordinateSystem->y1();
	vector z1 = m_pCoordinateSystem->z1();

	D3DXMATRIX A( x1.x(), x1.y(), x1.z(), 0.0f,
				  y1.x(), y1.y(), y1.z(), 0.0f,
				  z1.x(), z1.y(), z1.z(), 0.0f,
				   0.0f ,  0.0f ,  0.0f , 1.0f );

	D3DXMATRIX A_inv;

	D3DXMatrixInverse( &A_inv, NULL, &A );
	
	float pos1_x = pos.x() * A_inv(0,0) +
				   pos.y() * A_inv(1,0) +
				   pos.z() * A_inv(2,0);

	float pos1_y = pos.x() * A_inv(0,1) +
				   pos.y() * A_inv(1,1) +
				   pos.z() * A_inv(2,1);

	float pos1_z = pos.x() * A_inv(0,2) +
				   pos.y() * A_inv(1,2) +
				   pos.z() * A_inv(2,2);

	vector pos1( pos1_x, pos1_y, pos1_z );

	return pos1;
}


void Camera::update()
{
	Body::update();

	m_ViewPort.left   = int( m_pos.x() ) - m_P0.x;
	m_ViewPort.right  = int( m_pos.x() ) + m_P0.x;
	m_ViewPort.top    = int( m_pos.y() ) - m_P0.y;
	m_ViewPort.bottom = int( m_pos.y() ) + m_P0.y;
}