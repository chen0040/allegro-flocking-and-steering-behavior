#include "Obstacle.h"

Obstacle::Obstacle()
: BaseEntity()
{

}

Obstacle::~Obstacle()
{

}

void Obstacle::Update(const long& lElapsedTicks)
{

}

void Obstacle::Render(BITMAP *pScreen)
{
	int iColor=m_iColor;
	if(m_is_tagged)
	{
		iColor=makecol(255, 255, 0);
	}
	circlefill(pScreen, m_position.get_x(), m_position.get_y(), m_bounding_radius, iColor);
}