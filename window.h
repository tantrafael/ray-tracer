#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <windows.h>


class Window
{
	public:
		Window();
		Window( HINSTANCE hInst, HWND hWnd, int width, int height );
		~Window();

		HINSTANCE hInst();
		HWND	  hWnd();
		int		  width();
		int		  height();

	protected:
		HINSTANCE m_hInst;
		HWND	  m_hWnd;
		int		  m_width;
		int		  m_height;
};


#endif