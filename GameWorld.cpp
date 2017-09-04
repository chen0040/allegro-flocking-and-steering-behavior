#include "GameWorld.h"
#include "VectorS.h"
#include "Obstacle.h"
#include "SteeringBehavior.h"
#include "RandomS.h"

GameWorld::GameWorld()
: m_iWidth(640)
, m_iHeight(480)
, m_pPath(NULL)
, m_bShowPath(false)
{

}

GameWorld::~GameWorld()
{
	remove_agents();
	remove_obstacles();

	if(m_pPath != NULL)
	{
		delete m_pPath;
	}
}

void GameWorld::remove_agents()
{
	std::vector<Vehicle*>::iterator pos_agent;
	for(pos_agent=m_agents.begin(); pos_agent!=m_agents.end(); ++pos_agent)
	{
		delete *pos_agent;
	}
	m_agents.clear();
}

void GameWorld::remove_obstacles()
{
	std::vector<BaseEntity*>::iterator pos;
	BaseEntity* pEntity;
	for(pos=m_obstacles.begin(); pos!=m_obstacles.end(); ++pos)
	{
		pEntity=*pos;
		delete pEntity;
	}
	m_obstacles.clear();
}

void GameWorld::HandleInput()
{
	if(key[KEY_0])
	{
		m_bShowPath=false;
		test_interpose();
	}
	if(key[KEY_1])
	{
		m_bShowPath=false;
		test_seek();
	}
	else if(key[KEY_2])
	{
		m_bShowPath=false;
		test_flee();
	}
	else if(key[KEY_3])
	{
		m_bShowPath=false;
		test_wander();
	}
	else if(key[KEY_4])
	{
		test_pursuit();
	}
	else if(key[KEY_5])
	{
		m_bShowPath=false;
		test_evade();
	}
	else if(key[KEY_6])
	{
		m_bShowPath=false;
		test_obstacle_avoidance();
	}
	else if(key[KEY_7])
	{
		m_bShowPath=false;
		test_hide();
	}
	else if(key[KEY_8])
	{
		m_bShowPath=true;
		test_path_follow();
	}
	else if(key[KEY_9])
	{
		m_bShowPath=false;
		test_flock();
	}
}

void GameWorld::test_flee()
{
	remove_agents();
	remove_obstacles();

	VectorS target(m_iWidth/2, m_iHeight/2);
	VectorS vehicle_position=target+VectorS(RandEngine->nextDouble(), RandEngine->nextDouble());

	Vehicle* pAgent=new Vehicle();
	pAgent->set_position(vehicle_position);
	pAgent->set_max_force(4.0);
	pAgent->set_max_speed(100);
	pAgent->set_mass(0.01);
	pAgent->get_steering()->flee_on(target);
	pAgent->set_world(this);

	m_agents.push_back(pAgent);
	
	AddObstacle(target, 10, makecol(255, 0, 0));
}

void GameWorld::test_seek()
{
	remove_agents();
	remove_obstacles();

	VectorS target(m_iWidth/2, m_iHeight/2);
	VectorS vehicle_position=target + VectorS(RandEngine->nextDouble(100, 300), RandEngine->nextDouble(-100, -300));

	Vehicle* pAgent=new Vehicle();
	pAgent->set_position(vehicle_position);
	pAgent->set_max_force(4.0);
	pAgent->set_max_speed(100);
	pAgent->set_mass(0.01);
	pAgent->get_steering()->seek_on(target);
	pAgent->set_world(this);

	m_agents.push_back(pAgent);
	
	AddObstacle(target, 10, makecol(255, 0, 0));
}

void GameWorld::test_wander()
{
	remove_agents();
	remove_obstacles();

	VectorS center(m_iWidth/2, m_iHeight/2);
	VectorS vehicle_position=center + VectorS(RandEngine->nextDouble(100, 300), RandEngine->nextDouble(-100, -300));

	Vehicle* pAgent=new Vehicle();
	pAgent->set_position(vehicle_position);
	pAgent->set_max_force(4.0);
	pAgent->set_max_speed(100);
	pAgent->set_mass(0.01);
	pAgent->get_steering()->wander_on();
	pAgent->set_world(this);

	m_agents.push_back(pAgent);
}

void GameWorld::test_path_follow()
{
	remove_agents();
	remove_obstacles();

	if(m_pPath != NULL)
	{
		delete m_pPath;
	}

	VectorS center(m_iWidth/2, m_iHeight/2);
	VectorS vehicle_position1=center + VectorS(RandEngine->nextDouble(100, 300), RandEngine->nextDouble(-100, -300));
	m_pPath=new PathS(10, 100, 500, 100, 300);

	Vehicle* pAgent1=new Vehicle();
	pAgent1->set_position(vehicle_position1);
	pAgent1->set_max_force(4.0);
	pAgent1->set_max_speed(100);
	pAgent1->set_mass(0.01);
	pAgent1->get_steering()->wander_on();
	pAgent1->set_world(this);

	m_agents.push_back(pAgent1);

	pAgent1->get_steering()->wander_on();
	pAgent1->get_steering()->path_follow_on(m_pPath);
}

void GameWorld::test_pursuit()
{
	remove_agents();
	remove_obstacles();

	VectorS center(m_iWidth/2, m_iHeight/2);
	VectorS vehicle_position1=center + VectorS(RandEngine->nextDouble(100, 300), RandEngine->nextDouble(-100, -300));

	Vehicle* pAgent1=new Vehicle();
	pAgent1->set_position(vehicle_position1);
	pAgent1->set_max_force(4.0);
	pAgent1->set_max_speed(100);
	pAgent1->set_mass(0.01);
	pAgent1->get_steering()->wander_on();
	pAgent1->set_world(this);

	m_agents.push_back(pAgent1);

	VectorS vehicle_position2=center + VectorS(RandEngine->nextDouble(100, 300), RandEngine->nextDouble(100, 300));
	Vehicle* pAgent2=new Vehicle();
	pAgent2->set_position(vehicle_position2);
	pAgent2->set_max_force(4.0);
	pAgent2->set_max_speed(100);
	pAgent2->set_mass(0.01);
	pAgent2->get_steering()->pursuit_on(pAgent1);
	pAgent2->set_world(this);
	
	m_agents.push_back(pAgent2);

}

void GameWorld::test_flock()
{
	remove_agents();
	remove_obstacles();

	VectorS center(m_iWidth/2, m_iHeight/2);
	VectorS vehicle_position1=center + VectorS(RandEngine->nextDouble(100, 300), RandEngine->nextDouble(-100, -300));
	

	Vehicle* pAgent1=NULL;

	Vehicle* pAgent2=new Vehicle();
	pAgent2->set_position(vehicle_position1);
	pAgent2->set_max_force(4.0);
	pAgent2->set_max_speed(100);
	pAgent2->set_mass(0.01);
	pAgent2->get_steering()->wander_on();
	pAgent2->set_world(this);
	pAgent2->set_color(makecol(255, 0, 0));
	pAgent2->get_steering()->obstacle_avoidance_on();

	m_agents.push_back(pAgent2);
	
	for(int i=0; i<10; i++)
	{
		vehicle_position1=center + VectorS(RandEngine->nextDouble(100, 300), RandEngine->nextDouble(-100, -300));
		pAgent1=new Vehicle();
		pAgent1->set_position(vehicle_position1);
		pAgent1->set_max_force(4.0);
		pAgent1->set_max_speed(100);
		pAgent1->set_mass(0.01);
		pAgent1->get_steering()->wander_on();
		pAgent1->get_steering()->separation_on();
		pAgent1->get_steering()->alignment_on();
		pAgent1->get_steering()->cohesion_on();
		pAgent1->get_steering()->evade_on(pAgent2);
		//pAgent1->get_steering()->obstacle_avoidance_on();
		pAgent1->set_world(this);

		m_agents.push_back(pAgent1);
	}

	pAgent1->set_color(makecol(255, 255, 0));
	pAgent2->get_steering()->pursuit_on(pAgent1);
}

void GameWorld::test_interpose()
{
	remove_agents();
	remove_obstacles();

	VectorS center(m_iWidth/2, m_iHeight/2);
	VectorS vehicle_position1=center + VectorS(RandEngine->nextDouble(100, 300), RandEngine->nextDouble(-100, -300));

	Vehicle* pAgent1=new Vehicle();
	pAgent1->set_position(vehicle_position1);
	pAgent1->set_max_force(6.0);
	pAgent1->set_max_speed(150);
	pAgent1->set_mass(0.005);
	pAgent1->set_world(this);

	m_agents.push_back(pAgent1);

	VectorS vehicle_position2=center + VectorS(RandEngine->nextDouble(100, 300), RandEngine->nextDouble(100, 300));
	Vehicle* pAgent2=new Vehicle();
	pAgent2->set_position(vehicle_position2);
	pAgent2->set_max_force(4.0);
	pAgent2->set_max_speed(100);
	pAgent2->set_mass(0.01);
	pAgent2->set_world(this);
	
	m_agents.push_back(pAgent2);

	pAgent1->get_steering()->wander_on();
	pAgent2->get_steering()->wander_on();

	VectorS vehicle_position3=center + VectorS(RandEngine->nextDouble(100, 300), RandEngine->nextDouble(100, 300));
	Vehicle* pAgent3=new Vehicle();
	pAgent3->set_position(vehicle_position3);
	pAgent3->set_max_force(4.0);
	pAgent3->set_max_speed(100);
	pAgent3->set_mass(0.01);
	pAgent3->set_world(this);
	
	m_agents.push_back(pAgent3);

	pAgent3->set_color(makecol(255, 255, 0));
	pAgent3->get_steering()->interpose_on(pAgent1, pAgent2);

}

void GameWorld::test_evade()
{
	remove_agents();
	remove_obstacles();

	VectorS center(m_iWidth/2, m_iHeight/2);
	VectorS vehicle_position1=center + VectorS(RandEngine->nextDouble(100, 300), RandEngine->nextDouble(-100, -300));

	Vehicle* pAgent1=new Vehicle();
	pAgent1->set_position(vehicle_position1);
	pAgent1->set_max_force(6.0);
	pAgent1->set_max_speed(150);
	pAgent1->set_mass(0.005);
	pAgent1->set_world(this);

	m_agents.push_back(pAgent1);

	VectorS vehicle_position2=center + VectorS(RandEngine->nextDouble(100, 300), RandEngine->nextDouble(100, 300));
	Vehicle* pAgent2=new Vehicle();
	pAgent2->set_position(vehicle_position2);
	pAgent2->set_max_force(4.0);
	pAgent2->set_max_speed(100);
	pAgent2->set_mass(0.01);
	pAgent2->set_world(this);
	
	m_agents.push_back(pAgent2);

	pAgent1->get_steering()->wander_on();
	pAgent1->get_steering()->evade_on(pAgent2);
	pAgent2->get_steering()->pursuit_on(pAgent1);
}

void GameWorld::test_obstacle_avoidance()
{
	remove_agents();
	remove_obstacles();

	VectorS center(m_iWidth/2, m_iHeight/2);
	VectorS vehicle_position1=center + VectorS(RandEngine->nextDouble(100, 300), RandEngine->nextDouble(-100, -300));

	Vehicle* pAgent1=new Vehicle();
	pAgent1->set_position(vehicle_position1);
	pAgent1->set_max_force(6.0);
	pAgent1->set_max_speed(150);
	pAgent1->set_mass(0.01);
	pAgent1->set_world(this);

	m_agents.push_back(pAgent1);

	pAgent1->get_steering()->wander_on();
	pAgent1->get_steering()->obstacle_avoidance_on();
}

void GameWorld::test_hide()
{
	remove_agents();
	remove_obstacles();

	VectorS center(m_iWidth/2, m_iHeight/2);
	VectorS vehicle_position1=center + VectorS(RandEngine->nextDouble(100, 300), RandEngine->nextDouble(-100, -300));

	Vehicle* pAgent1=new Vehicle();
	pAgent1->set_position(vehicle_position1);
	pAgent1->set_max_force(6.0);
	pAgent1->set_max_speed(150);
	pAgent1->set_mass(0.01);
	pAgent1->set_world(this);

	m_agents.push_back(pAgent1);

	VectorS vehicle_position2=center + VectorS(RandEngine->nextDouble(100, 300), RandEngine->nextDouble(100, 300));
	Vehicle* pAgent2=new Vehicle();
	pAgent2->set_position(vehicle_position2);
	pAgent2->set_max_force(4.0);
	pAgent2->set_max_speed(100);
	pAgent2->set_mass(0.01);
	pAgent2->set_world(this);
	
	m_agents.push_back(pAgent2);

	pAgent1->get_steering()->wander_on();
	pAgent1->get_steering()->hide_on(pAgent2);
	pAgent1->get_steering()->obstacle_avoidance_on();

	pAgent2->get_steering()->wander_on();
	pAgent2->get_steering()->pursuit_on(pAgent1);
	pAgent2->get_steering()->obstacle_avoidance_on();
}

void GameWorld::Update(const long& lElapsedTicks)
{
	HandleInput();

	std::vector<BaseEntity*>::iterator pos;
	BaseEntity* pEntity;
	for(pos=m_obstacles.begin(); pos!=m_obstacles.end(); ++pos)
	{
		pEntity=*pos;
		pEntity->Update(lElapsedTicks);
	}

	std::vector<Vehicle*>::iterator pos_agent;
	for(pos_agent=m_agents.begin(); pos_agent!=m_agents.end(); ++pos_agent)
	{
		(*pos_agent)->Update(lElapsedTicks);
	}
}

void GameWorld::Render(BITMAP* pScreen)
{
	if(m_bShowPath && m_pPath != NULL)
	{
		m_pPath->Render(pScreen);
	}

	std::vector<BaseEntity*>::iterator pos;
	BaseEntity* pEntity;
	for(pos=m_obstacles.begin(); pos!=m_obstacles.end(); ++pos)
	{
		pEntity=*pos;
		pEntity->Render(pScreen);
	}

	std::vector<Vehicle*>::iterator pos_agent;
	for(pos_agent=m_agents.begin(); pos_agent!=m_agents.end(); ++pos_agent)
	{
		(*pos_agent)->Render(pScreen);
	}
}

void GameWorld::AddObstacle(const VectorS& position, double bounding_radius, int color)
{
	Obstacle* pOb=new Obstacle();
	pOb->set_bounding_radius(bounding_radius);
	pOb->set_position(position);
	pOb->set_color(color);
	pOb->set_world(this);
	
	m_obstacles.push_back(pOb);
}

void GameWorld::MouseLeftButtonDown(const int& iX, const int& iY)
{
	double bounding_radius=RandEngine->nextDouble(10, 30);
	AddObstacle(VectorS(iX, iY), bounding_radius, makecol(255, 0, 0));
}

