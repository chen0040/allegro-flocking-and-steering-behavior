#include "ParameterLoader.h"

ParameterLoader::ParameterLoader()
: m_dMinDetectionBoxLength(40.0)
, m_dMultWander(1.0)
, m_dMultObstacleAvoidance(10.0)
, m_dMultiHide(1.0)
, m_dMultPathFollow(0.05)
, m_dWayPointSeekDistSq(20)
, m_dMultSeparation(3.0)
, m_dMultAlignment(1.0)
, m_dMultCohesion(1.0)
, m_dViewDistance(50.0)
{

}

ParameterLoader* ParameterLoader::GetInstance()
{
	static ParameterLoader theInstance;
	return &theInstance;
}