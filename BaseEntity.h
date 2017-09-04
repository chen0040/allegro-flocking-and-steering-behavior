#ifndef _H_BASE_ENTITY_H
#define _H_BASE_ENTITY_H

#include "VectorS.h"
#include <vector>
#include <allegro.h>
#include <winalleg.h>

class GameWorld;

class BaseEntity
{
public:
	BaseEntity();
	virtual ~BaseEntity();

public:
	virtual void Update(const long& lElapsedTicks)=0;
	virtual void Render(BITMAP* pScreen)=0;

public:
	VectorS get_position() const { return m_position; }
	void set_position(VectorS v) { m_position = v; } 
	double get_bounding_radius() const { return m_bounding_radius; }
	void set_bounding_radius(double radius) { m_bounding_radius = radius; }

public:
	GameWorld* get_world() { return m_pWorld; }
	void set_world(GameWorld* pWorld) { m_pWorld=pWorld; }

public:
	void untag() { m_is_tagged=false; }
	void tag() { m_is_tagged=true; }
	bool is_tagged() const { return m_is_tagged; }

protected:
	VectorS m_position;
	double m_bounding_radius;
	bool m_is_tagged;
	GameWorld* m_pWorld;
};
#endif