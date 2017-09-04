#ifndef _H_VECTOR_S_H
#define _H_VECTOR_S_H

class VectorS
{
public:
	VectorS();
	VectorS(double x, double y, double z=0.0);
	virtual ~VectorS();

public:
	VectorS(const VectorS& rhs);
	VectorS& operator=(const VectorS& rhs);

public:
	double length() const;
	double lengthSq() const;
	VectorS normalize() const;
	VectorS perp() const;
	VectorS& operator+=(const VectorS& rhs);
	VectorS& operator-=(const VectorS& rhs);
	VectorS& operator*=(double scalar_value);
	VectorS& operator/=(double scalar_value);
	double dot(const VectorS& rhs) const;
	VectorS& truncate(double scalar_value);
	VectorS& rotate(double rad);
	VectorS& translate(double x, double y, double z=0.0);
	VectorS& zero();

public:
	double get_x() const { return m_x; }
	double get_y() const { return m_y; }
	double get_z() const { return m_z; }

	void set_x(double x) { m_x=x; }
	void set_y(double y) { m_y=y; }
	void set_z(double z) { m_z=z; }

private:
	double m_x;
	double m_y;
	double m_z;
};

VectorS operator+(const VectorS& v1, const VectorS& v2);
VectorS operator-(const VectorS& v1, const VectorS& v2);
VectorS operator*(const VectorS& v1, double scalar_value);
VectorS operator*(double scalar_value, const VectorS& v1); 
VectorS operator/(const VectorS& v1, double scalar_value);

#endif