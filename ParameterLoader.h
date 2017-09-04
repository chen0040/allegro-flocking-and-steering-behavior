#ifndef _H_PARAMETER_LOADER_H
#define _H_PARAMETER_LOADER_H

class ParameterLoader
{
public:
	virtual ~ParameterLoader() { }
	static ParameterLoader* GetInstance();

private:
	ParameterLoader();
	ParameterLoader(const ParameterLoader& rhs) { }
	ParameterLoader& operator=(const ParameterLoader& rhs) { return *this; }

public:
	double m_dMinDetectionBoxLength;
	double m_dWallDetectionFeelerLength;
	double m_dWayPointSeekDistSq;
	double m_dViewDistance;
	double m_dMultWallAvoidance;
	double m_dMultObstacleAvoidance;
	double m_dMultWander;
	double m_dMultiHide;
	double m_dMultPathFollow;
	double m_dMultSeparation;
	double m_dMultAlignment;
	double m_dMultCohesion;
};

#define Prm ParameterLoader::GetInstance()

#endif