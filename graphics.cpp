#include "Graphics.h"
#include "Game.h"


Graphics::Graphics()
{
	m_pGame		  = NULL;
	m_pD3D		  = NULL;
	m_pD3Ddevice  = NULL;
}

Graphics::~Graphics()
{
	m_TermD3D();
}


void Graphics::init( Game *pGame )
{
	m_pGame = pGame;
	m_InitD3D();
}


//-----------------------------------------------------------------------------
// m_InitD3D()
//-----------------------------------------------------------------------------
HRESULT Graphics::m_InitD3D()
{
	HRESULT hr;
	
	// Create D3D object
	m_pD3D = Direct3DCreate8( D3D_SDK_VERSION );
	
	if( m_pD3D == NULL )
		return E_FAIL;

	// Get current desktop display mode
	D3DDISPLAYMODE d3ddm;
	hr = m_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );

	if( FAILED(hr) )
        return E_FAIL;

	// Set parameters
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof(d3dpp) );

	d3dpp.BackBufferWidth  = m_pGame->window()->width();
	d3dpp.BackBufferHeight = m_pGame->window()->height();
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = m_pGame->window()->hWnd();
	d3dpp.Windowed = 1;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;	// För att kunna skriva
														// till backbuffer

	// Create D3D device
	hr = m_pD3D->CreateDevice( D3DADAPTER_DEFAULT,
							   D3DDEVTYPE_HAL,
							   m_pGame->window()->hWnd(),
							   D3DCREATE_SOFTWARE_VERTEXPROCESSING,
							   &d3dpp,
							   &m_pD3Ddevice );
	if( FAILED(hr) )
		return E_FAIL;

	return S_OK;
}


//-----------------------------------------------------------------------------
// m_TermD3D()
//-----------------------------------------------------------------------------
void WINAPI Graphics::m_TermD3D()
{
	if (m_pD3D)
	{
		if (m_pD3Ddevice)
		{
			m_pD3Ddevice->Release();
			m_pD3Ddevice = NULL;
		}
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}


//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
void Graphics::ClearBuffer( Color *pColor )
{
	m_pD3Ddevice->Clear( 0, NULL, D3DCLEAR_TARGET,
						 pColor->getColor(), 1.0f, 0 );
}


void Graphics::BeginScene()
{
	m_pD3Ddevice->BeginScene();
}


void Graphics::EndScene()
{
	m_pD3Ddevice->EndScene();
}


void Graphics::Present()
{
	m_pD3Ddevice->Present( NULL, NULL, NULL, NULL );
}


void Graphics::PutPixel( int x, int y, Color &color )
{
	IDirect3DSurface8 *pDstSurf;
	m_pD3Ddevice->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pDstSurf );

	D3DLOCKED_RECT lDstRect;
	pDstSurf->LockRect( &lDstRect, NULL, 0 );
	unsigned long bpp = 4;
	unsigned char *DstBuffer = (unsigned char *) lDstRect.pBits;
	unsigned long dst_basepos = 0, dst_bufpos = 0;

	dst_bufpos = y * lDstRect.Pitch + x * bpp;
	D3DCOLOR D3Dcolor = color.getColor();
	memcpy( &DstBuffer[dst_bufpos], &D3Dcolor, bpp );

	pDstSurf->UnlockRect();
}
