#include "MathS.h"

MathS* MathS::GetInstance()
{
	static MathS theInstance;
	return &theInstance;
}

bool MathS::LineIntersect2D(const VectorS& A1, const VectorS& B1, const VectorS& A2, const VectorS& B2, double& distance, VectorS& point)
{
	// let line1: P1=A1 + u1 * (B1 - A1)
	// let line2: P2=A2 + u2 * (B2 - A2)
	// suppose intersection then P1=P2
	// A1 + u1 * (B1 - A1) = A2 + u2 * (B2 - A2)
	// (1): xA1 + u1 * (xB1 - xA1) = xA2 + u2 * (xB2 - xA2)
	// (2): yA1 + u1 * (yB1 - yA1) = yA2 + u2 * (yB2 - yA2)
	// from (1), we have
	// (3): u2 = [ u1 * (xB1 - xA1) + (xA1 - xA2) ] / (xB2 - xA2)
	// sub (3) into (2), we have
	// yA1 + u1 * (yB1 - yA1) = yA2 + (yB2 - yA2) * [ u1 * (xB1 - xA1) + (xA1 - xA2) ] / (xB2 - xA2)
	// (yA1 - yA2) * (xB2 - xA2) + u1 * (yB1 - yA1) * ( xB2 - xA2) = u1 * (xB1 - xA1 ) * (yB2 - yA2) + (xA1 - xA2) * (yB2 - yA2)
	// u1= [ (yA1 - yA2) * (xB2 - xA2) - (xA1 - xA2) * (yB2 - yA2) ] / [ (xB1 - xA1) * (yB2 - yA2) - (yB1 - yA1) * (xB2 - xA2) ]
	// u2= [ (xB1 - xA1) * (yA1 - yA2) - (xA1 - xA2) * (yB1 - yA1) ] / [ (xB1 - xA1) * (yB2 - yA2) - (yB1 - yA1) * (xB2 - xA2) ]
	double xA1=A1.get_x();
	double yA1=A1.get_y();
	double xB1=B1.get_x();
	double yB1=B1.get_y();
	double xA2=A2.get_x();
	double yA2=A2.get_y();
	double xB2=B2.get_x();
	double yB2=B2.get_y();

	double denom=(xB1 - xA1) * (yB2 - yA2) - (yB1 - yA1) * (xB2 - xA2);

	if(denom==0)
	{
		return false;
	}

	double num1=(yA1 - yA2) * (xB2 - xA2) - (xA1 - xA2) * (yB2 - yA2);
	double num2=(xB1 - xA1) * (yA1 - yA2) - (xA1 - xA2) * (yB1 - yA1);
	double u1 = num1 / denom;
	double u2 = num2 / denom;

	if(u1 > 0 && u1 < 1 && u2 > 0 && u2 < 1)
	{
		point=A1 + (B1 - A1) * u1;
		distance=(point - A1).length();
		return true;
	}

	return false;
}