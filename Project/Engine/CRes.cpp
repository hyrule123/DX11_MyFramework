#include "pch.h"
#include "CRes.h"

CRes::CRes(eRES_TYPE _type)
	: m_Type(_type)
	, m_iRefCount(0)
{
}

CRes::CRes(const CRes& _other)
	: m_Type(_other.m_Type)
	, m_iRefCount(0)
	, m_strKey(_other.m_strKey)
	, m_strRelativePath(_other.m_strRelativePath)
{
}

CRes::~CRes()
{
}

void CRes::Release()
{
	--m_iRefCount;

	if (0 == m_iRefCount)
	{
		delete this;
	}
}
