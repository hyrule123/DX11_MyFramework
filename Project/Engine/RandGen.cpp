#include "pch.h"
#include "RandGen.h"

#include "TimeMgr.h"

std::mt19937 m_RandEngine = std::mt19937((unsigned int)time(0));

