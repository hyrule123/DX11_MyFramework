#include "pch.h"
#include "CRandMgr.h"

#include "CTimeMgr.h"

DEFINITION_SINGLETON(CRandMgr);

CRandMgr::CRandMgr()
	: m_RandEngine((unsigned int)time(0))
{
}

CRandMgr::~CRandMgr()
{
}
