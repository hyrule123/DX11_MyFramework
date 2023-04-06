#include "pch.h"
#include "CScript.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"


CScript::CScript(std::type_index _TypeIndex)
	: m_TypeIndex(_TypeIndex)
	, m_pHolder()
{
}

CScript::CScript(const CScript& _other)
	: CEntity(_other)
	, m_TypeIndex(_other.m_TypeIndex)
	, m_pHolder()
{
}

CScript::~CScript()
{
}
