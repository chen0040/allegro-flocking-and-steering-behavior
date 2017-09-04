#ifndef _H_MATH_S_H
#define _H_MATH_S_H

#include "VectorS.h"

const double PI=3.1415926;

class MathS
{
public:
	virtual ~MathS() { }
	static MathS* GetInstance();

private:
	MathS() { }
	MathS(const MathS& rhs) { }
	MathS& operator=(const MathS& rhs) { return *this;}

public:
	double PI() const { return 3.1415926;; }
	bool LineIntersect2D(const VectorS& line1p1, const VectorS& line1p2, const VectorS& line2p1, const VectorS& line2p2, double& distance, VectorS& point);
};

#define MathEngine MathS::GetInstance()

#endif