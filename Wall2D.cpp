#include "Wall2D.h"

Wall2D::Wall2D(const VectorS& vA, const VectorS& vB)
{

}

Wall2D::Wall2D(const Wall2D& rhs)
{
	m_vA=rhs.m_vA;
	m_vB=rhs.m_vB;
	m_vNormal=rhs.m_vNormal;
}

Wall2D& Wall2D::operator =(const Wall2D &rhs)
{
	m_vA=rhs.m_vA;
	m_vB=rhs.m_vB;
	m_vNormal=rhs.m_vNormal;

	return *this;
}

void Wall2D::calculate_normal()
{
	VectorS vWall=m_vB - m_vA;
	vWall=vWall.normalize();
	m_vNormal=VectorS(-vWall.get_y(), vWall.get_x());
}