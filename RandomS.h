#ifndef _H_RANDOM_S_H
#define _H_RANDOM_S_H

class RandomS
{
public:
	virtual ~RandomS() { }
	static RandomS* GetInstance();

private:
	RandomS();
	RandomS(const RandomS& rhs) { }
	RandomS& operator=(const RandomS& rhs) { return *this; }

public:
	double nextDouble() const;
	double nextDouble(double lower_bound, double upper_bound) const;
	int nextInt(int upper_bound) const;
};

#define RandEngine RandomS::GetInstance()
#endif