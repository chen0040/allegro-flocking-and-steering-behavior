#ifndef _H_GAME_ENGINE_H
#define _H_GAME_ENGINE_H

#include <allegro.h>
#include <winalleg.h>

class GameEngine
{
public:
	GameEngine();
	virtual ~GameEngine();

public:
	bool Init(const int& iWidth, const int& iHeight, const bool& bWindowed);
	void Start();

protected:
	void DoThink();
	void DoRender();
	void HandleInput();

protected:
	virtual void Think(const long& lElapsedTicks) { }
	virtual void Render(BITMAP* buffer) { }

protected:
	bool m_bLeftButtonDown;
	bool m_bRightButtonDown;
public:
	bool IsLeftButtonDown() const { return m_bLeftButtonDown; }
	bool IsRightButtonDown() const { return m_bRightButtonDown; }

protected:
	int m_mouse_x;
	int m_mouse_y;
public:
	int GetMouseX() const { return m_mouse_x; }
	int GetMouseY() const { return m_mouse_y; }

protected:
	virtual void MouseLeftButtonDown(const int& iX, const int& iY) { }
	virtual void MouseRightButtonDown(const int& iX, const int& iY) { }
	virtual void MouseLeftButtonUp(const int& iX, const int& iY) { }
	virtual void MouseRightButtonUp(const int& iX, const int& iY) { }
	virtual void MouseMoved(const int& iXRel, const int& iYRel) { }

protected:
	void Allegro_Flip(BITMAP* buffer);

private:
	BITMAP* m_pScreen;

private:
	int m_iWidth;
	int m_iHeight;
	int m_iWidthInit;
	int m_iHeightInit;
public:
	int GetWidth() const { return m_iWidth; }
	int GetHeight() const { return m_iHeight; }
	int GetWidthInit() const { return m_iWidthInit; }
	int GetHeightInit() const { return m_iHeightInit; }

private:
	long m_lLastTick;
	long m_lFPSTickCounter;
	long m_lFPSCounter;
	long m_lFPSCurrent;
public:
	long GetFPS() const { return m_lFPSCurrent; }

private:
	bool m_bQuit;
	bool m_bWindowed;
};
#endif