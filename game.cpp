#include "Game.h"

Game::Game()
{
	m_pWindow   = NULL;
	m_pGraphics = NULL;
	m_pWorld	= NULL;
	m_pCamera	= NULL;
	m_pInput	= NULL;
}

Game::Game( Window *pWindow )
{
	m_pWindow = pWindow;
	m_pGraphics = new Graphics();
	m_pGraphics->init( this );
	m_pInput = new Input();
	m_pInput->init( this );
	m_pWorld = new World();
	m_pWorld->init( this );

	m_pWorld->draw();
}

Game::~Game()
{
	delete m_pWorld;
	delete m_pInput;
	delete m_pGraphics;
}

Window*	  Game::window()   { return m_pWindow; }
Graphics* Game::graphics() { return m_pGraphics; }
World*	  Game::world()	   { return m_pWorld; }
Camera*	  Game::camera()   { return m_pCamera; }
Input*	  Game::input()	   { return m_pInput; }

void Game::update()
{
	m_pInput->read();
//	m_pWorld->update();
//	m_pWorld->draw();
}
