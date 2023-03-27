#include "pch.h"
#include "CRandMgr.h"

#include "CTimeMgr.h"

CRandMgr::CRandMgr()
	: m_RandEngine((unsigned int)time(0))
{
}

CRandMgr::~CRandMgr()
{
}
