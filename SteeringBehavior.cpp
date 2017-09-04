#include "SteeringBehavior.h"
#include "Vehicle.h"
#include "RandomS.h"
#include <cmath>
#include "ParameterLoader.h"
#include <cfloat>
#include "GameWorld.h"
#include "MathS.h"
#include "PathS.h"
#include "DebugS.h"

SteeringBehavior::SteeringBehavior(Vehicle* pVehicle)
: m_pVehicle(pVehicle)
, m_dPanicDistance(100)
, m_dWanderRadius(3.0)
, m_dWanderJitter(1.0)
, m_dWanderDistance(5.0)
{
	for(int i=0; i<BEHAVIOR_COUNT; i++)
	{
		m_behavior_on[i]=false;
	}

	double theta= PI * 2 * RandEngine->nextDouble();
	m_vWanderTarget=VectorS(m_dWanderRadius* cos(theta), m_dWanderRadius * sin(theta));
}

SteeringBehavior::~SteeringBehavior()
{

}

VectorS SteeringBehavior::Calculate()
{
	VectorS force;

	if(On(BEHAVIOR_SEPARATION) || On(BEHAVIOR_ALIGNMENT) || On(BEHAVIOR_COHESION))
	{
		m_pVehicle->tag_agents(Prm->m_dViewDistance);
	}

	if(On(BEHAVIOR_OBSTACLE_AVOIDANCE))
	{
		VectorS steering_force=obstacle_avoidance(m_pVehicle->get_world()->get_obstacles()) * Prm->m_dMultObstacleAvoidance;
		// dout << "obstacle avoidance force: " << steering_force.get_x() << ", " << steering_force.get_y() << std::endl;
		if(!accumulate_force(force, steering_force)) return force;
	}

	if(On(BEHAVIOR_SEPARATION))
	{
		VectorS steering_force=separation(m_pVehicle->get_world()->get_agents()) * Prm->m_dMultSeparation;
		if(!accumulate_force(force, steering_force)) return force;
	}

	if(On(BEHAVIOR_ALIGNMENT))
	{
		VectorS steering_force=alignment(m_pVehicle->get_world()->get_agents()) * Prm->m_dMultAlignment;
		if(!accumulate_force(force, steering_force)) return force;
	}

	if(On(BEHAVIOR_COHESION))
	{
		VectorS steering_force=cohesion(m_pVehicle->get_world()->get_agents()) * Prm->m_dMultCohesion;
		if(!accumulate_force(force, steering_force)) return force;
	}

	if(On(BEHAVIOR_INTERPOSE))
	{
		VectorS steering_force=interpose(m_pInterposeAgent1, m_pInterposeAgent2);
		if(!accumulate_force(force, steering_force)) return force;
	}

	if(On(BEHAVIOR_HIDE))
	{
		VectorS steering_force=hide(m_pHideTarget, m_pVehicle->get_world()->get_obstacles()) * Prm->m_dMultiHide;

		if(!accumulate_force(force, steering_force)) return force;
	}

	if(On(BEHAVIOR_PATH_FOLLOW))
	{
		VectorS steering_force=follow_path(m_pPathToFollow) * Prm->m_dMultPathFollow;

		if(!accumulate_force(force, steering_force)) return force;
	}

	if(On(BEHAVIOR_SEEK))
	{
		VectorS steering_force=seek(m_pos_seek_target);
		//dout << "steering_force: " << steering_force.get_x() << ", " << steering_force.get_y() << std::endl;

		if(!accumulate_force(force, steering_force)) return force;
	}
	if(On(BEHAVIOR_FLEE))
	{
		VectorS steering_force=flee(m_pos_flee_target);

		// dout << "flee force: " << steering_force.get_x() << ", " << steering_force.get_y() << std::endl;
		if(!accumulate_force(force, steering_force)) return force;
	}
	if(On(BEHAVIOR_WANDER))
	{
		VectorS steering_force=wander() * Prm->m_dMultWander;

		// dout << "wander force: " << steering_force.get_x() << ", " << steering_force.get_y() << std::endl;
		if(!accumulate_force(force, steering_force)) return force;
	}
	if(On(BEHAVIOR_PURSUIT))
	{
		VectorS steering_force=pursuit(m_pPursuitTarget);

		// dout << "pursuit force: " << steering_force.get_x() << ", " << steering_force.get_y() << std::endl;
		if(!accumulate_force(force, steering_force)) return force;
	}
	if(On(BEHAVIOR_EVADE))
	{
		VectorS steering_force=pursuit(m_pEvadeTarget);

		// dout << "evade force: " << steering_force.get_x() << ", " << steering_force.get_y() << std::endl;
		if(!accumulate_force(force, steering_force)) return force;
	}

	return force;
}

bool SteeringBehavior::accumulate_force(VectorS& current_total_force, const VectorS& force_to_add)
{
	double current_force_mag=current_total_force.length();

	double remaining_force_mag=m_pVehicle->get_max_force() - current_force_mag;

	if(remaining_force_mag <= 0) return false;

	double force_to_add_mag=force_to_add.length();

	if(force_to_add_mag >= remaining_force_mag)
	{
		current_total_force+=(force_to_add.normalize() * remaining_force_mag);
	}
	else
	{
		current_total_force+=force_to_add;
	}

	return true;
}

void SteeringBehavior::set_vehicle(Vehicle* pVehicle)
{
	m_pVehicle=pVehicle;
}

VectorS SteeringBehavior::seek(const VectorS& target)
{
	VectorS toTarget=target - m_pVehicle->get_position();

	VectorS desiredVelocity=toTarget.normalize() * m_pVehicle->get_max_speed();

	return desiredVelocity - m_pVehicle->get_velocity();
}

VectorS SteeringBehavior::flee(const VectorS& target)
{
	VectorS awayTarget=m_pVehicle->get_position() - target;
	double panic_distance_sq=m_dPanicDistance * m_dPanicDistance;

	if(awayTarget.lengthSq() > panic_distance_sq)
	{
		return VectorS(0, 0);
	}

	VectorS desiredVelocity = awayTarget.normalize() * m_pVehicle->get_max_speed();

	return desiredVelocity - m_pVehicle->get_velocity();
}

VectorS SteeringBehavior::arrive(const VectorS& target, double deceleration)
{
	VectorS toTarget=target - m_pVehicle->get_position();
	double distance=toTarget.length();
	if(distance > 0)
	{
		double speed=distance / deceleration;
		speed=speed > m_pVehicle->get_max_speed() ? m_pVehicle->get_max_speed() : speed;

		return toTarget.normalize() * speed - m_pVehicle->get_velocity();
	}

	return VectorS(0, 0);
}

VectorS SteeringBehavior::pursuit(const Vehicle* pTarget)
{
	VectorS toTarget=pTarget->get_position() - m_pVehicle->get_position();
	double relativeHeading=m_pVehicle->get_heading().dot(pTarget->get_heading());

	if(m_pVehicle->get_heading().dot(toTarget) > 0 && relativeHeading < -0.98)
	{
		return seek(pTarget->get_position());
	}

	double lookAheadTime=toTarget.length() / (m_pVehicle->get_max_speed() + pTarget->get_speed());

	// dout << "lookAheadTime: " << lookAheadTime << std::endl;

	return seek(pTarget->get_position() + pTarget->get_velocity() * lookAheadTime);

}

VectorS SteeringBehavior::evade(const Vehicle* pTarget)
{
	VectorS toTarget=pTarget->get_position() - m_pVehicle->get_position();

	double lookAheadTime=toTarget.length() / (m_pVehicle->get_max_speed() + pTarget->get_speed());

	return flee(pTarget->get_position() + pTarget->get_velocity() * lookAheadTime);
}

void SteeringBehavior::pursuit_on(Vehicle* pVehicle)
{
	m_pPursuitTarget=pVehicle;
	m_behavior_on[BEHAVIOR_PURSUIT]=true;
}

void SteeringBehavior::evade_on(Vehicle* pVehicle)
{
	m_pEvadeTarget=pVehicle;
	m_behavior_on[BEHAVIOR_EVADE]=true;
}


VectorS SteeringBehavior::wander()
{
	m_vWanderTarget+=VectorS(RandEngine->nextDouble(-1, 1) * m_dWanderJitter, RandEngine->nextDouble(-1, 1) * m_dWanderJitter);
	m_vWanderTarget=m_vWanderTarget.normalize() * m_dWanderRadius;

	// dout << "m_vWanderTarget: " << m_vWanderTarget.get_x() << ", " << m_vWanderTarget.get_y() << std::endl;

	VectorS worldTarget=m_pVehicle->PointToWorldSpace(m_vWanderTarget+VectorS(m_dWanderRadius, 0));

	// dout << "worldTarget: " << worldTarget.get_x() << ", " << worldTarget.get_y() << std::endl;
	// dout << "position: " << m_pVehicle->get_position().get_x() << ", " << m_pVehicle->get_position().get_y() << std::endl;

	return worldTarget - m_pVehicle->get_position();
}

VectorS SteeringBehavior::obstacle_avoidance(const std::vector<BaseEntity*>& obstacles)
{
	m_dBoxLength=Prm->m_dMinDetectionBoxLength + (m_pVehicle->get_speed() / m_pVehicle->get_max_speed()) * Prm->m_dMinDetectionBoxLength;

	m_pVehicle->tag_obstacles(m_dBoxLength);

	BaseEntity* closest_obstacle=NULL;
	double distance_to_closest_obstacle=DBL_MAX;
	VectorS local_position_of_closest_obstacle;

	std::vector<BaseEntity*>::const_iterator curOb=obstacles.begin();
	while(curOb != obstacles.end())
	{
		if((*curOb)->is_tagged())
		{
			VectorS local_position=m_pVehicle->PointToLocalSpace((*curOb)->get_position());

			if(local_position.get_x() > 0)
			{
				double ExpandedRadius=(*curOb)->get_bounding_radius()+m_pVehicle->get_bounding_radius();

				if(abs(local_position.get_y()) < ExpandedRadius)
				{
					double cx=local_position.get_x();
					double cy=local_position.get_y();

					double SquarePart=sqrt(ExpandedRadius * ExpandedRadius - cy * cy);
					double ip=cx - SquarePart;
					if(ip <= 0)
					{
						ip=cx + SquarePart;
					}

					if(distance_to_closest_obstacle > ip)
					{
						distance_to_closest_obstacle=ip;
						local_position_of_closest_obstacle=local_position;
						closest_obstacle=*curOb;
					}
				}
			}
			(*curOb)->untag();
		}

		++curOb;
	}

	VectorS steering_force;

	if(closest_obstacle != NULL)
	{
		closest_obstacle->tag();
		double multiplier = 1.0 + (m_dBoxLength - local_position_of_closest_obstacle.get_x()) / m_dBoxLength; 
		const double braking_weight=0.2;
		double obstacle_bounding_radius=closest_obstacle->get_bounding_radius() ;
		steering_force.set_y((obstacle_bounding_radius - local_position_of_closest_obstacle.get_y()) * multiplier);
		steering_force.set_x((obstacle_bounding_radius - local_position_of_closest_obstacle.get_x()) * braking_weight);
		// dout << "avoidance steering force: " << steering_force.get_x() << ", " << steering_force.get_y() << std::endl;
	}

	m_pVehicle->VectorToWorldSpace(steering_force);


	return steering_force;
}

void SteeringBehavior::hide_on(Vehicle* pVehicle)
{
	m_pHideTarget=pVehicle;
	m_behavior_on[BEHAVIOR_HIDE]=true;
}

void SteeringBehavior::path_follow_on(PathS* pPath)
{
	m_pPathToFollow=pPath;
	m_behavior_on[BEHAVIOR_PATH_FOLLOW]=true;
}

void SteeringBehavior::interpose_on(Vehicle* pAgent1, Vehicle* pAgent2)
{
	m_pInterposeAgent1=pAgent1;
	m_pInterposeAgent2=pAgent2;

	m_behavior_on[BEHAVIOR_INTERPOSE]=true;
}

VectorS SteeringBehavior::wall_avoidance(const std::vector<Wall2D>& walls)
{
	VectorS feelers[3];

	feelers[0]=m_pVehicle->get_position() + m_pVehicle->get_heading() * Prm->m_dWallDetectionFeelerLength;
	VectorS feeler_direction=m_pVehicle->get_heading();
	feeler_direction.rotate(PI * 1.75);
	feelers[1]=m_pVehicle->get_position() + feeler_direction * Prm->m_dWallDetectionFeelerLength * 0.5;
	feeler_direction=m_pVehicle->get_heading();
	feeler_direction.rotate(PI * 0.25);
	feelers[2]=m_pVehicle->get_position() + feeler_direction * Prm->m_dWallDetectionFeelerLength * 0.5;

	VectorS steering_force;

	int closest_wall=-1;
	int sf=-1;
	double closest_distance=DBL_MAX;
	VectorS closest_point;

	for(int i=0; i<3; i++)
	{
		for(size_t w=0; w!=walls.size(); w++)
		{
			double dist=DBL_MAX;
			VectorS point;
			if(MathEngine->LineIntersect2D(m_pVehicle->get_position(), feelers[i], walls[w].get_start(), walls[w].get_end(), dist, point))
			{
				if(dist < closest_distance)
				{
					closest_wall=w;
					closest_distance=dist;
					closest_point=point;
					sf=i;
				}
			}
		}
	}

	if(closest_wall > 0)
	{
		double mag=(feelers[sf]-m_pVehicle->get_position()).length();
		steering_force=walls[closest_wall].get_normal() * mag;
	}

	return steering_force;

}

VectorS SteeringBehavior::interpose(const Vehicle* pAgentA, const Vehicle* pAgentB)
{
	VectorS MidPoint=(pAgentA->get_position() + pAgentB->get_position()) / 2.0;

	double time_to_reach_midpoint=(MidPoint - m_pVehicle->get_position()).length() / m_pVehicle->get_max_speed();

	VectorS posA=pAgentA->get_position() + pAgentA->get_velocity() * time_to_reach_midpoint;
	VectorS posB=pAgentB->get_position() + pAgentB->get_velocity() * time_to_reach_midpoint;

	MidPoint=(posA + posB) / 2.0;

	return arrive(MidPoint, 0.9);
}

VectorS SteeringBehavior::hide(const Vehicle* pTarget, const std::vector<BaseEntity*>& obstacles)
{
	BaseEntity* best_ob_to_hide=NULL;
	double distance_to_best_ob=DBL_MAX;
	VectorS best_hiding_spot;
	
	const double distance_to_boundary=30.0;
	std::vector<BaseEntity*>::const_iterator curOb=obstacles.begin();
	while(curOb != obstacles.end())
	{
		BaseEntity* pOb=*curOb;

		double dist=pOb->get_bounding_radius() + distance_to_boundary;

		VectorS hiding_spot=(pOb->get_position() - pTarget->get_position()).normalize() * dist + pOb->get_position();

		double distance_to_hiding_spot=(hiding_spot - m_pVehicle->get_position()).lengthSq();

		if(distance_to_hiding_spot < distance_to_best_ob)
		{
			best_ob_to_hide=pOb;
			distance_to_best_ob=distance_to_hiding_spot;
			best_hiding_spot=hiding_spot;
		}
		curOb++;
	}

	if(best_ob_to_hide!=NULL)
	{
		return arrive(best_hiding_spot, 0.9);
	}
	
	return evade(pTarget);

}

VectorS SteeringBehavior::follow_path(PathS* pPath)
{
	VectorS point;
	double distSq=(m_pVehicle->get_position() - pPath->get_current_way_point()).lengthSq();
	if(distSq < Prm->m_dWayPointSeekDistSq)
	{
		pPath->set_next_way_point();
	}

	if(pPath->finished())
	{
		return arrive(pPath->get_current_way_point());
	}

	return seek(pPath->get_current_way_point());
}

VectorS SteeringBehavior::offset_pursuit(const Vehicle* pLeader, const VectorS& offset)
{
	VectorS world_offset=pLeader->PointToWorldSpace(offset);

	double lookAheadTime=(world_offset - m_pVehicle->get_position()).length() / (m_pVehicle->get_max_speed() + pLeader->get_speed());

	return arrive(pLeader->get_position() + lookAheadTime * pLeader->get_velocity(), 0.9);
}

VectorS SteeringBehavior::separation(const std::vector<Vehicle*>& neighbors)
{
	VectorS steering_force;
	std::vector<Vehicle*>::const_iterator pos;
	for(pos=neighbors.begin(); pos!=neighbors.end(); ++pos)
	{
		Vehicle* pNeighbor=*pos;
		if(pNeighbor!=m_pVehicle && pNeighbor->is_tagged())
		{
			VectorS toAgent=m_pVehicle->get_position() - pNeighbor->get_position();
			steering_force+=toAgent / toAgent.lengthSq();
		}
	}

	return steering_force;
}

void SteeringBehavior::flee_on(const VectorS& posTarget)
{
	m_behavior_on[BEHAVIOR_FLEE]=true;
	m_pos_flee_target=posTarget;
}

VectorS SteeringBehavior::alignment(const std::vector<Vehicle*>& neighbors)
{
	VectorS averageHeading;
	std::vector<Vehicle*>::const_iterator pos;
	int neighbor_count=0;
	for(pos=neighbors.begin(); pos!=neighbors.end(); ++pos)
	{
		Vehicle* pNeighbor=*pos;
		if(pNeighbor!=m_pVehicle && pNeighbor->is_tagged())
		{
			averageHeading+=pNeighbor->get_heading();
			neighbor_count++;
		}
	}

	if(neighbor_count>0)
	{
		averageHeading /= (double)neighbor_count;
		return averageHeading - m_pVehicle->get_heading();
	}

	return averageHeading;
}

void SteeringBehavior::seek_on(const VectorS& posTarget)
{
	m_behavior_on[BEHAVIOR_SEEK]=true;
	m_pos_seek_target=posTarget;
}

VectorS SteeringBehavior::cohesion(const std::vector<Vehicle*>& neighbors)
{
	VectorS com, steering_force;
	int neighbor_count=0;
	std::vector<Vehicle*>::const_iterator pos;
	for(pos=neighbors.begin(); pos!=neighbors.end(); ++pos)
	{
		Vehicle* pNeighbor=*pos;
		if(pNeighbor != m_pVehicle && pNeighbor->is_tagged())
		{
			com+=pNeighbor->get_position();
			neighbor_count++;
		}
	}

	if(neighbor_count > 0)
	{
		com /= (double)neighbor_count;
		steering_force=seek(com);
	}

	return steering_force;
}