#include "Vehicle.h"
#include "GameWorld.h"
#include "SteeringBehavior.h"
#include "DebugS.h"

Vehicle::Vehicle() 
: MovingEntity()
{
	m_iColor=makecol(255, 255, 255);
	m_pSteering=new SteeringBehavior(this);
}

Vehicle::~Vehicle()
{
	if(m_pSteering != NULL)
	{
		delete m_pSteering;
	}
}

void Vehicle::Update(const long& lElapsedTicks)
{
	double elapsed_time=(double)lElapsedTicks / 1000.0;

	VectorS force=m_pSteering->Calculate();
	VectorS accelaration=force / m_dMass;
	m_vVelocity+=accelaration * elapsed_time;

	// dout << "m_vVelocity: " << m_vVelocity.get_x() << ", " << m_vVelocity.get_y() << std::endl;

	m_vVelocity.truncate(m_dMaxSpeed);

	if(m_vVelocity.length() != 0)
	{
		m_vHeading=m_vVelocity.normalize();
		m_vSide=m_vHeading.perp();
	}

	m_position += m_vVelocity * elapsed_time;

	wrap_around(m_position);

	this->enforce_non_overlapping_penetration(m_pWorld->get_agents());

	// dout << "position: " << m_position.get_x() << ", " << m_position.get_y() << std::endl;
}

void Vehicle::enforce_non_overlapping_penetration(std::vector<Vehicle*>& entities)
{
	std::vector<Vehicle*>::iterator pos;
	for(pos=entities.begin(); pos!=entities.end(); ++pos)
	{
		Vehicle* pEntity=*pos;
		if(pEntity==this)
		{
			continue;
		}
		VectorS toEntity=pEntity->get_position() - this->get_position();
		double distance=toEntity.length();
		double penetration_distance=pEntity->get_bounding_radius() + this->get_bounding_radius() - distance;
		if(penetration_distance > 0)
		{
			pEntity->set_position(pEntity->get_position() + toEntity / distance * penetration_distance);
		}
	}
}

void Vehicle::Render(BITMAP* pScreen)
{
	VectorS vHead=m_position+m_vHeading*m_bounding_radius*1.5;
	VectorS vSide1=m_position+m_vSide*m_bounding_radius / 2;
	VectorS vSide2=m_position-m_vSide*m_bounding_radius / 2;

	// dout << "m_vHeading: " << m_vHeading.get_x() << ", " << m_vHeading.get_y() << std::endl;
	//dout << "x1: " << vSide1.get_x() << ", y1: " << vSide1.get_y() << ", x2: " <<  vHead.get_x() << ", y2: " <<  vHead.get_y() << ", x3: " << vSide2.get_x() << ", y3: " << vSide2.get_y() << std::endl;
	triangle(pScreen, vSide1.get_x(), vSide1.get_y(), vHead.get_x(), vHead.get_y(), vSide2.get_x(), vSide2.get_y(), m_iColor);

	if(m_pSteering->On(SteeringBehavior::BEHAVIOR_OBSTACLE_AVOIDANCE))
	{
		VectorS v1=m_position + m_vSide*m_bounding_radius;
		VectorS v2=m_position + m_vSide*m_bounding_radius + m_vHeading * m_pSteering->get_detection_box_length();
		VectorS v3=m_position - m_vSide*m_bounding_radius + m_vHeading * m_pSteering->get_detection_box_length();
		VectorS v4=m_position - m_vSide*m_bounding_radius;
		line(pScreen, v1.get_x(), v1.get_y(), v2.get_x(), v2.get_y(), makecol(125, 125, 125));
		line(pScreen, v3.get_x(), v3.get_y(), v4.get_x(), v4.get_y(), makecol(125, 125, 125));
		line(pScreen, v2.get_x(), v2.get_y(), v3.get_x(), v3.get_y(), makecol(125, 125, 125));
		line(pScreen, v4.get_x(), v4.get_y(), v1.get_x(), v1.get_y(), makecol(125, 125, 125));
	}
}

VectorS Vehicle::PointToWorldSpace(const VectorS& local_point) const
{
	// m_vHeading=[cos(t), sin(t)]
	// m_vSide=[-sin(t), cos(t)]
	// vpi is obtained by rotating local_point=[x, y] through (t) angle then translate by m_position
	// vpi=[xcos(t)-ysin(t), ycos(t)+xsin(t)]=[xcos(t)-ysin(t), ycos(t)+xsin(t)]
	// vpi=[x * m_vHeading.get_x() + y * m_vSide.get_x(), x * m_vHeading.get_y() + y * m_vSide.get_y()]
	double x=local_point.get_x() * m_vHeading.get_x() + local_point.get_y() * m_vSide.get_x();
	double y=local_point.get_x() * m_vHeading.get_y() + local_point.get_y() * m_vSide.get_y();

	// dout << "m_vHeading: " << m_vHeading.get_x() << ", " << m_vHeading.get_y() << std::endl;

	VectorS vpi(x, y);

	// dout << "vpi: " << vpi.get_x() << ", " << vpi.get_y() << std::endl;

	vpi+=m_position;
	return vpi;
}

VectorS Vehicle::PointToLocalSpace(const VectorS& world_point) const
{
	// m_vHeading=[cos(t), sin(t)]
	// m_vSide=[-sin(t), cos(t)]
	// vpi is obtained by first translate the world_point=[x1, y1] by (-m_position=[-x, -y]) then rotating through (-t) angle
	// vpi' = world_point - m_position = [x', y']=[x1-x, y1-y]
	// vpi = [x'cos(-t)-y'sin(-t), y'cos(-t)+x'sin(-t)]=[x1cos(t) + y1sin(t) - { xcos(t) + ysin(t) }, y1cos(t) - x1sin(t) - {ycos(t) - xsin(t)}]
	// vpi = [m_vHeading.dot(world_point), m_vSide.dot(world_point)] - [m_vHeading.dot(m_position), m_vSide.dot(m_position)]
	// vpi = [x'cos(-t)-y'sin(-t), y'cos(-t)+x'sin(-t)]=[m_vHeading.dot(vpi'), m_vSide.dot(vpi')]

	VectorS vpi=world_point;

	vpi-=m_position;

	double x=m_vHeading.dot(vpi);
	double y=m_vSide.dot(vpi);

	vpi.set_x(x);
	vpi.set_y(y);

	return vpi;

}

VectorS Vehicle::VectorToWorldSpace(const VectorS& local_vector) const
{
	// m_vHeading=[cos(t), sin(t)]
	// m_vSide=[-sin(t), cos(t)]
	// local_vector=[x, y]
	// obtained by rotating local_vector through an angle (t)
	// world_vector=[xcos(t)-ysin(t), ycos(t)+xsin(t)]
	// world_vector.x=x * m_vHeading.x + y * m_vSide.x;
	// world_vector.y=x * m_vHeading.y + y * m_vSide.y;
	double x=local_vector.get_x();
	double y=local_vector.get_y();

	double world_x=x * m_vHeading.get_x() + y * m_vSide.get_x();
	double world_y=x * m_vHeading.get_y() + y * m_vSide.get_y();
	
	return VectorS(world_x, world_y);
}

void Vehicle::wrap_around(VectorS& position)
{
	double current_x=position.get_x();
	double current_y=position.get_y();

	double game_width=m_pWorld->cxClient();
	double game_height=m_pWorld->cyClient();

	if(current_x < 0)
	{
		current_x += game_width;
	}
	else if(current_x > game_width)
	{
		current_x -= game_width;
	}

	if(current_y < 0)
	{
		current_y += game_height;
	}
	else if(current_y > game_height)
	{
		current_y -= game_height;
	}

	position.set_x(current_x);
	position.set_y(current_y);
}

void Vehicle::tag_agents(double view_distance)
{
	std::vector<Vehicle*>& agents=m_pWorld->get_agents();

	std::vector<Vehicle*>::iterator pos_agent=agents.begin();
	while(pos_agent!=agents.end())
	{
		BaseEntity* pAgent=*pos_agent;
		pAgent->untag();
		if(pAgent != this)
		{
			double radius=view_distance + pAgent->get_bounding_radius();
			if((pAgent->get_position() - m_position).lengthSq() < radius * radius)
			{
				pAgent->tag();
			}
		}
		pos_agent++;
	}
}

void Vehicle::tag_obstacles(double view_distance)
{
	std::vector<BaseEntity*>& obs=m_pWorld->get_obstacles();
	std::vector<BaseEntity*>::iterator pos;
	for(pos=obs.begin(); pos!=obs.end(); ++pos)
	{
		BaseEntity* pOst=*pos;
		pOst->untag();
		if(pOst==this)
		{
			continue;
		}
		double radius=view_distance+pOst->get_bounding_radius();
		VectorS to=pOst->get_position() - m_position;
		if(to.lengthSq() < radius * radius)
		{
			pOst->tag();
		}
	}
}

