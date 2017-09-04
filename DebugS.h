#ifndef _H_DEBUG_S_H
#define _H_DEBUG_S_H

#include <fstream>
#include <iostream>

class DebugS
{
public:
	virtual ~DebugS();
	static DebugS* GetInstance();

private:
	DebugS();
	DebugS(const DebugS& rhs) { }
	DebugS& operator=(const DebugS& rhs) { return *this; }

public:
	std::ofstream& cout() { return m_out; }

private:
	std::ofstream m_out;
};

#define dout (DebugS::GetInstance())->cout()
#endif