#include "Window.h"


Window::Window()
{}

Window::Window( HINSTANCE hInst, HWND hWnd, int width, int height )
{
	m_hInst  = hInst;
	m_hWnd   = hWnd;
	m_width  = width;
	m_height = height;
}

Window::~Window()
{}

HINSTANCE Window::hInst()  { return m_hInst;  }
HWND	  Window::hWnd()   { return m_hWnd;   }
int		  Window::width()  { return m_width;  }
int		  Window::height() { return m_height; }
