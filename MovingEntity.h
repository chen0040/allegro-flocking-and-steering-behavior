#ifndef _H_MOVING_ENTITY_H
#define _H_MOVING_ENTITY_H

#include "BaseEntity.h"

class MovingEntity : public BaseEntity
{
public:
	MovingEntity();
	virtual ~MovingEntity();

public:
	VectorS get_velocity() const { return m_vVelocity; }
	double get_mass() const { return m_dMass; }
	double get_max_turn_rate() const { return m_dMaxTurnRate; }
	double get_max_speed() const { return m_dMaxSpeed; }
	double get_max_force() const { return m_dMaxForce; }
	VectorS get_heading() const { return m_vHeading; }
	double get_speed() const { return m_vVelocity.length(); }

public:
	void set_max_force(double force) { m_dMaxForce=force; }
	void set_max_speed(double speed) { m_dMaxSpeed=speed; }
	void set_mass(double mass) { m_dMass=mass; }

protected:
	double m_dMass;
	double m_dMaxTurnRate;
	double m_dMaxSpeed;
	double m_dMaxForce;

	VectorS m_vVelocity;
	VectorS m_vHeading;
	VectorS m_vSide;
};
#endif