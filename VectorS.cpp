#include "VectorS.h"
#include <cmath>
#include "DebugS.h"

VectorS::VectorS()
: m_x(0)
, m_y(0)
, m_z(0)
{

}

VectorS::VectorS(double x, double y, double z)
: m_x(x)
, m_y(y)
, m_z(z)
{

}

VectorS::~VectorS()
{

}

VectorS::VectorS(const VectorS& rhs)
{
	this->m_x=rhs.m_x;
	this->m_y=rhs.m_y;
	this->m_z=rhs.m_z;
}

VectorS& VectorS::operator=(const VectorS& rhs)
{
	m_x=rhs.m_x;
	m_y=rhs.m_y;
	m_z=rhs.m_z;

	return *this;
}

double VectorS::length() const
{
	return sqrt(m_x*m_x + m_y*m_y + m_z * m_z);
}

double VectorS::lengthSq() const
{
	return m_x*m_x + m_y*m_y + m_z * m_z;
}

VectorS VectorS::normalize() const
{
	double len=this->length();

	if(len==0)
	{
		return VectorS(0, 0, 0);
	}

	VectorS v(m_x / len, m_y / len, m_z / len);
	
	return v;
}

VectorS VectorS::perp() const
{
	return VectorS(-m_y, m_x, m_z);
}

VectorS& VectorS::operator+=(const VectorS& rhs)
{
	m_x+=rhs.m_x;
	m_y+=rhs.m_y;
	m_z+=rhs.m_z;

	return *this;
}

VectorS& VectorS::operator-=(const VectorS& rhs)
{
	m_x-=rhs.m_x;
	m_y-=rhs.m_y;
	m_z-=rhs.m_z;

	return *this;
}

VectorS& VectorS::operator*=(double scalar_value)
{
	m_x*=scalar_value;
	m_y*=scalar_value;
	m_z*=scalar_value;

	return *this;
}

VectorS& VectorS::operator/=(double scalar_value)
{
	m_x/=scalar_value;
	m_y/=scalar_value;
	m_z/=scalar_value;

	return *this;
}

double VectorS::dot(const VectorS& rhs) const
{
	return this->m_x * rhs.m_x + this->m_y * rhs.m_y + this->m_z * rhs.m_z;
}

VectorS& VectorS::truncate(double max_value)
{
	double l=length();

	if(l > max_value)
	{
		m_x = m_x * max_value / l;
		m_y = m_y * max_value / l;
		m_z = m_z * max_value / l;
	}

	return *this;
}

VectorS operator+(const VectorS& v1, const VectorS& v2)
{
	VectorS v(v1);
	v += v2;
	return v;
}

VectorS operator-(const VectorS& v1, const VectorS& v2)
{
	VectorS v=v1;
	v -= v2;

	//dout << "v: " << v.get_x() << ", " << v.get_y() << std::endl;

	return v;
}

VectorS operator*(const VectorS& v1, double scalar_value)
{
	VectorS v=v1;
	v *= scalar_value;
	return v;
}

VectorS operator*(double scalar_value, const VectorS& v1)
{
	VectorS v=v1;
	v *= scalar_value;
	return v;
}

VectorS operator/(const VectorS& v1, double scalar_value)
{
	VectorS v=v1;
	v /= scalar_value;
	return v;
}

VectorS& VectorS::rotate(double rad)
{
	double x=m_x * cos(rad) - m_y * sin(rad);
	double y=m_y * cos(rad) + m_x* sin(rad);

	m_x=x;
	m_y=y;

	return *this;
}

VectorS& VectorS::translate(double x, double y, double z)
{
	m_x+=x;
	m_y+=y;
	m_z+=z;

	return *this;
}

VectorS& VectorS::zero()
{
	m_x=0;
	m_y=0;
	m_z=0;

	return *this;
}
