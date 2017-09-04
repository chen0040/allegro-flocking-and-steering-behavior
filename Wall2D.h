#ifndef _H_WALL_2D_H
#define _H_WALL_2D_H

#include "VectorS.h"

class Wall2D
{
public:
	Wall2D(const VectorS& vA, const VectorS& vB);
	virtual ~Wall2D() { }

public:
	Wall2D(const Wall2D& rhs);
	Wall2D& operator=(const Wall2D& rhs);

public:
	VectorS get_normal() const { return m_vNormal; }
	VectorS get_start() const { return m_vA; }
	VectorS get_end() const { return m_vB; }

protected:
	void calculate_normal();

private:
	VectorS m_vNormal;
	VectorS m_vA;
	VectorS m_vB;
};
#endif