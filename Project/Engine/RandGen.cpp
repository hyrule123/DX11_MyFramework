#include "pch.h"
#include "RandGen.h"

#include "cTimeMgr.h"

std::mt19937 RandGen::m_RandEngine = std::mt19937((unsigned int)time(0));

