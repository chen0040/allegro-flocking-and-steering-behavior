#include "XSEngine.h"
#include <string>
#include <sstream>

XSEngine::XSEngine()
: GameEngine()
{

}

XSEngine::~XSEngine()
{

}

void XSEngine::Think(const long& lElapsedTicks)
{
	m_world.Update(lElapsedTicks);
}

void XSEngine::Render(BITMAP* pScreen)
{
	m_world.Render(pScreen);
}

void XSEngine::MouseLeftButtonDown(const int& iX, const int& iY)
{
	m_world.MouseLeftButtonDown(iX, iY);
}