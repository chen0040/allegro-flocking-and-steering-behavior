#ifndef _H_XS_ENGINE_H
#define _H_XS_ENGINE_H

#include "GameEngine.h"
#include "GameWorld.h"

class XSEngine : public GameEngine
{
public:
	XSEngine();
	virtual ~XSEngine();

protected:
	virtual void Think(const long& lElapsedTicks);
	virtual void Render(BITMAP* pScreen);

protected:
	virtual void MouseLeftButtonDown(const int& iX, const int& iY);

protected:
	GameWorld m_world;
};
#endif