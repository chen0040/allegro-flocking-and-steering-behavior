#include "MovingEntity.h"

MovingEntity::MovingEntity()
: BaseEntity()
, m_dMaxSpeed(150)
, m_dMaxForce(2)
, m_dMaxTurnRate(0)
, m_dMass(0.01)
, m_vHeading(1, 0)
, m_vSide(0, 1)
{

}

MovingEntity::~MovingEntity()
{

}