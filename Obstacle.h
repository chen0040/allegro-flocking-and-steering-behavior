#ifndef _H_OBSTACLE_H
#define _H_OBSTACLE_H

#include "BaseEntity.h"

class Obstacle : public BaseEntity
{
public:
	Obstacle();
	virtual ~Obstacle();

public:
	virtual void Render(BITMAP* pScreen);
	virtual void Update(const long& lElapsedTicks);

public:
	int get_color() const { return m_iColor; }
	void set_color(int iColor) { m_iColor=iColor; }

private:
	int m_iColor;
};
#endif