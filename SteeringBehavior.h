#ifndef _H_STEERING_BEHAVIOR_H
#define _H_STEERING_BEHAVIOR_H

#include "VectorS.h"
#include <vector>
#include "Wall2D.h"

class BaseEntity;
class Vehicle;
class PathS;

class SteeringBehavior
{
public:
	SteeringBehavior(Vehicle* pVehicle);
	virtual ~SteeringBehavior();

public:
	virtual VectorS Calculate();
	void set_vehicle(Vehicle* pVehicle);

protected:
	virtual VectorS seek(const VectorS& target);
	virtual VectorS flee(const VectorS& target);
	virtual VectorS arrive(const VectorS& target, double deceleration=0.3);
	virtual VectorS pursuit(const Vehicle* pTarget);
	virtual VectorS evade(const Vehicle* pTarget);
	virtual VectorS wander();
	virtual VectorS obstacle_avoidance(const std::vector<BaseEntity*>& ostacles);
	virtual VectorS wall_avoidance(const std::vector<Wall2D>& walls);
	virtual VectorS interpose(const Vehicle* pAgentA, const Vehicle* pAgentB);
	virtual VectorS hide(const Vehicle* pTarget, const std::vector<BaseEntity*>& obstacles);
	virtual VectorS follow_path(PathS* pPath);
	virtual VectorS offset_pursuit(const Vehicle* pLeader, const VectorS& offset);
	virtual VectorS separation(const std::vector<Vehicle*>& neighbors);
	virtual VectorS alignment(const std::vector<Vehicle*>& neighbors);
	virtual VectorS cohesion(const std::vector<Vehicle*>& neighbors);

protected:
	VectorS m_pos_seek_target;
public:
	void seek_on(const VectorS& posTarget);
	void seek_off() { m_behavior_on[BEHAVIOR_SEEK]=false; }
	
protected:
	VectorS m_pos_flee_target;
public:
	void flee_on(const VectorS& posTarget);
	void flee_off() { m_behavior_on[BEHAVIOR_FLEE]=false; }

public:
	void wander_on() { m_behavior_on[BEHAVIOR_WANDER]=true; }
	void wander_off() { m_behavior_on[BEHAVIOR_WANDER]=false; }

protected:
	Vehicle* m_pPursuitTarget;
public:
	void pursuit_on(Vehicle* pVehicle);
	void pursuit_off() { m_behavior_on[BEHAVIOR_PURSUIT]=false; }

protected:
	Vehicle* m_pEvadeTarget;
public:
	void evade_on(Vehicle* pVehicle);
	void evade_off() { m_behavior_on[BEHAVIOR_EVADE]=false; }

protected:
	Vehicle* m_pHideTarget;
public:
	void hide_on(Vehicle* pVehicle);
	void hide_off() { m_behavior_on[BEHAVIOR_HIDE]=false; }

protected:
	PathS* m_pPathToFollow;
public:
	void path_follow_on(PathS* pPath);
	void path_follow_off() { m_behavior_on[BEHAVIOR_PATH_FOLLOW]=false; }

protected:
	Vehicle* m_pInterposeAgent1;
	Vehicle* m_pInterposeAgent2;
public:
	void interpose_on(Vehicle* pAgent1, Vehicle* pAgent2);
	void interpose_off() { m_behavior_on[BEHAVIOR_INTERPOSE]=false; }

protected:
	double m_dBoxLength;
public:
	double get_detection_box_length() const { return m_dBoxLength; }
public:
	void obstacle_avoidance_on() { m_behavior_on[BEHAVIOR_OBSTACLE_AVOIDANCE]=true; }
	void obstacle_avoidance_off() { m_behavior_on[BEHAVIOR_OBSTACLE_AVOIDANCE]=false; }

public:
	void separation_on() { m_behavior_on[BEHAVIOR_SEPARATION]=true; }
	void separation_off() { m_behavior_on[BEHAVIOR_SEPARATION]=false; }

public:
	void cohesion_on() { m_behavior_on[BEHAVIOR_COHESION]=true; }
	void cohesion_off() {  m_behavior_on[BEHAVIOR_COHESION]=false; }

public:
	void alignment_on() { m_behavior_on[BEHAVIOR_ALIGNMENT]=true; }
	void alignment_off() { m_behavior_on[BEHAVIOR_ALIGNMENT]=false; }

protected:
	virtual bool accumulate_force(VectorS& current_total_force, const VectorS& force_to_add);

public:
	static const int BEHAVIOR_SEEK=0;
	static const int BEHAVIOR_SEPARATION=1;
	static const int BEHAVIOR_COHESION=2;
	static const int BEHAVIOR_ALIGNMENT=3;
	static const int BEHAVIOR_WALL_AVOIDANCE=4;
	static const int BEHAVIOR_OBSTACLE_AVOIDANCE=5;
	static const int BEHAVIOR_FLEE=6;
	static const int BEHAVIOR_WANDER=7;
	static const int BEHAVIOR_PURSUIT=8;
	static const int BEHAVIOR_EVADE=9;
	static const int BEHAVIOR_HIDE=10;
	static const int BEHAVIOR_PATH_FOLLOW=11;
	static const int BEHAVIOR_INTERPOSE=12;

	static const int BEHAVIOR_COUNT=13;

protected:
	bool m_behavior_on[BEHAVIOR_COUNT];
public:
	bool On(int behavior_type) { return m_behavior_on[behavior_type]; }

protected:
	Vehicle* m_pVehicle;

protected:
	double m_dPanicDistance;
public:
	void set_panic_distance(double distance) { m_dPanicDistance=distance; }
	double get_panic_distance() const { return m_dPanicDistance; }

protected:
	double m_dWanderDistance;
	double m_dWanderRadius;
	double m_dWanderJitter;
	VectorS m_vWanderTarget;
};
#endif