#ifndef _H_VEHICLE_H
#define _H_VEHICLE_H

#include "MovingEntity.h"

class SteeringBehavior;

class Vehicle : public MovingEntity
{
public:
	Vehicle();
	virtual ~Vehicle();

public:
	virtual void Update(const long& lElapsedTicks);
	virtual void Render(BITMAP* pScreen);

public:
	void wrap_around(VectorS& position);
	VectorS PointToWorldSpace(const VectorS& local_point) const;
	VectorS PointToLocalSpace(const VectorS& world_point) const;
	VectorS VectorToWorldSpace(const VectorS& local_vector) const;

public:
	SteeringBehavior* get_steering() { return m_pSteering; }

public:
	void tag_agents(double view_distance);
	void tag_obstacles(double view_distance);

protected:
	virtual void enforce_non_overlapping_penetration(std::vector<Vehicle*>& entities);

protected:
	int m_iColor;
public:
	void set_color(int iColor) { m_iColor=iColor; }
	int get_color() const { return m_iColor; }

protected:
	SteeringBehavior* m_pSteering;
};
#endif