#include "DebugS.h"

DebugS::DebugS()
{
	m_out.open("debug.txt");
}

DebugS::~DebugS()
{
	m_out.close();
}

DebugS* DebugS::GetInstance()
{
	static DebugS theInstance;
	return &theInstance;
}