#include "GameEngine.h"
#include "AllegroExitSystem.h"

GameEngine::GameEngine()
: m_lLastTick(0)
, m_lFPSCounter(0)
, m_lFPSTickCounter(0)
, m_lFPSCurrent(0)
, m_iWidth(640)
, m_iHeight(480)
, m_pScreen(NULL)
, m_bQuit(false)
, m_bWindowed(true)
, m_bLeftButtonDown(false)
, m_bRightButtonDown(false)
, m_mouse_x(0)
, m_mouse_y(0)
{

}

GameEngine::~GameEngine()
{

}

bool GameEngine::Init(const int &iWidth, const int &iHeight, const bool &bWindowed)
{
	if(allegro_init() != 0)
	{
		AllegroExitSystem("GameEngine::Init(const int& iWidth, const int& iHeight, const bool& bWindowed)", "Failed to init");
		return false;
	}

	if(install_keyboard() != 0)
	{
		AllegroExitSystem("GameEngine::Init(const int& iWidth, const int& iHeight, const bool& bWindowed)", "Failed to install keyboard");
		return false;
	}

	if(install_mouse() == -1)
	{
		AllegroExitSystem("GameEngine::Init(const int& iWidth, const int& iHeight, const bool& bWindowed)", "Failed to install mouse");
		return false;
	}

	if(install_timer()==-1)
	{
		AllegroExitSystem("GameEngine::Init(const int& iWidth, const int& iHeight, const bool& bWindowed)", "Failed to install timer");
		return false;
	}

	m_iWidth=iWidth;
	m_iHeight=iHeight;
	m_iWidthInit=iWidth;
	m_iHeightInit=iHeight;
	m_bWindowed=bWindowed;

	if(bWindowed)
	{
		set_gfx_mode(GFX_AUTODETECT_WINDOWED, iWidth, iHeight, 0, 0);
	}
	else
	{
		get_desktop_resolution(&m_iWidth, &m_iHeight);
		set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, m_iWidth, m_iHeight, 0, 0);
	}

	return true;

}

void GameEngine::Start()
{
	m_bQuit=false;
	m_lLastTick=timeGetTime();

	show_mouse(screen);
	m_pScreen=create_bitmap(m_iWidth, m_iHeight);

	m_mouse_x=mouse_x;
	m_mouse_y=mouse_y;

	while(!m_bQuit)
	{
		HandleInput();

		DoThink();
		DoRender();
	}

	destroy_bitmap(m_pScreen);
	m_pScreen=NULL;

	allegro_exit();
}

void GameEngine::HandleInput()
{
	if(key[KEY_ESC])
	{
		m_bQuit=true;
		return;
	}

	if(key[KEY_SPACE])
	{
		if(m_bWindowed)
		{
			m_bWindowed=false;
		}
		else
		{
			m_bWindowed=true;
		}
		
		if(m_bWindowed)
		{
			m_iWidth=m_iWidthInit;
			m_iHeight=m_iHeightInit;
			set_gfx_mode(GFX_AUTODETECT_WINDOWED, m_iWidthInit, m_iHeightInit, 0, 0);
		}
		else
		{
			get_desktop_resolution(&m_iWidth, &m_iHeight);
			set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, m_iWidth, m_iHeight, 0, 0);
		}

		destroy_bitmap(m_pScreen);
		m_pScreen=create_bitmap(m_iWidth, m_iHeight);
	}

	int mx=mouse_x;
	int my=mouse_y;
	if(mouse_b & 1)
	{
		if(!m_bLeftButtonDown)
		{
			m_bLeftButtonDown=true;
			MouseLeftButtonDown(mx, my);
		}
	}
	else
	{
		if(m_bLeftButtonDown)
		{
			m_bLeftButtonDown=false;
			MouseLeftButtonUp(mx, my);
		}
	}

	if(mouse_b & 2)
	{
		if(!m_bRightButtonDown)
		{
			m_bRightButtonDown=true;
			MouseRightButtonDown(mx, my);
		}
	}
	else
	{
		if(m_bRightButtonDown)
		{
			m_bRightButtonDown=false;
			MouseRightButtonUp(mx, my);
		}
	}


	if(m_mouse_x != mx || m_mouse_y != my)
	{
		int dx=mx - m_mouse_x;
		int dy=my - m_mouse_y;
		m_mouse_x=mx;
		m_mouse_y=my;

		MouseMoved(dx, dy);
	}
	
}

void GameEngine::DoThink()
{
	long lElapsedTicks=timeGetTime() - m_lLastTick;
	m_lLastTick=timeGetTime();

	Think(lElapsedTicks);

	m_lFPSTickCounter+=lElapsedTicks;
}

void GameEngine::DoRender()
{
	m_lFPSCounter++;
	if(m_lFPSTickCounter >= 1000)
	{
		m_lFPSCurrent=m_lFPSCounter;
		m_lFPSCounter=0;
		m_lFPSTickCounter=0;
	}

	clear_bitmap(m_pScreen);

	Render(m_pScreen);

	scare_mouse();
	acquire_screen();
	blit(m_pScreen, screen, 0, 0, 0, 0, m_pScreen->w, m_pScreen->h);
	release_screen();
	unscare_mouse();
}