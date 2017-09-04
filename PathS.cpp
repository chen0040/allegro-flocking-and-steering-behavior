#include "PathS.h"
#include "MathS.h"
#include "RandomS.h"
#include <cmath>
#include "DebugS.h"

PathS::PathS()
: m_bLooped(false)
, m_bEnded(true)
{

}

PathS::PathS(int numPoints, double minX, double maxX, double minY, double maxY)
: m_bLooped(false)
{
	this->create_random_way_points(numPoints, minX, maxX, minY, maxY);

	m_current_way_point=m_way_points.begin();
	m_current_point=*m_current_way_point;
	m_current_way_point++;
	if(m_current_way_point != m_way_points.end())
	{
		m_bEnded=false;
	}
	else
	{
		m_bEnded=true;
	}
}

void PathS::Render(BITMAP* pScreen)
{
	std::list<VectorS>::const_iterator pos;
	VectorS prev_way_point;
	VectorS first_way_point;
	for(pos=m_way_points.begin(); pos != m_way_points.end(); ++pos)
	{
		if(pos!=m_way_points.begin())
		{
			line(pScreen, prev_way_point.get_x(), prev_way_point.get_y(), (*pos).get_x(), (*pos).get_y(), makecol(200, 200, 200));
		}
		else
		{
			first_way_point=*pos;
		}
		prev_way_point=*pos;
		circle(pScreen, (*pos).get_x(), (*pos).get_y(), 2, makecol(255, 0, 255));
	}
	line(pScreen, prev_way_point.get_x(), prev_way_point.get_y(), first_way_point.get_x(), first_way_point.get_y(), makecol(200, 200, 200));
	
	// dout << "path rendered" << std::endl;
}

void PathS::create_random_way_points(int num_points, double minX, double maxX, double minY, double maxY)
{
	m_way_points.clear();

	double sector=MathEngine->PI() * 2 / num_points;

	double midX=(minX + maxX) / 2.0;
	double midY=(minY + maxY) / 2.0;

	double smaller=(midX < midY) ? midX : midY;
	smaller=fabs(smaller);

	for(int i=0; i<num_points; i++)
	{
		double rand_radius=RandEngine->nextDouble(smaller / 2.0, smaller);

		VectorS way_point(rand_radius, 0);

		way_point.rotate(i* sector);

		way_point+=VectorS(midX, midY);

		m_way_points.push_back(way_point);
	}

	m_current_way_point=m_way_points.begin();
	m_current_point=*m_current_way_point;
	m_current_way_point++;
	if(m_current_way_point != m_way_points.end())
	{
		m_bEnded=false;
	}
	else
	{
		m_bEnded=true;
	}
}

void PathS::set_next_way_point()
{
	if(!m_bEnded)
	{
		m_current_point=*m_current_way_point;
		m_current_way_point++;
		if(m_current_way_point==m_way_points.end())
		{
			if(m_bLooped)
			{
				m_current_way_point=m_way_points.begin();
			}
			else
			{
				m_bEnded=true;
			}
		}
	}
}