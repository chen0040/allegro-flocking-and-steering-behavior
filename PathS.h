#ifndef _H_PATH_S_H
#define _H_PATH_S_H

#include "VectorS.h"
#include <allegro.h>
#include <winalleg.h>
#include <list>

class PathS
{
public:
	PathS();
	PathS(int numPoints, double minX, double maxX, double minY, double maxY);
	virtual ~PathS() { }

public:
	void create_random_way_points(int num_points, double minX, double maxX, double minY, double maxY);
	const VectorS& get_current_way_point() const { return m_current_point; }
	void set_next_way_point();
	bool finished() const { return m_bEnded; }
	void loop_on() { m_bLooped=true; }
	void loop_off() { m_bLooped=false; }

public:
	virtual void Render(BITMAP* pScreen);

private:
	std::list<VectorS> m_way_points;
	std::list<VectorS>::iterator m_current_way_point;
	bool m_bLooped;
	VectorS m_current_point;
	bool m_bEnded;
};
#endif