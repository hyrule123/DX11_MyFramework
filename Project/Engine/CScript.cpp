#include "pch.h"
#include "CScript.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"


CScript::CScript(const std::type_index& _TypeIndex)
	: m_TypeIndex(_TypeIndex)
{
}

CScript::~CScript()
{
}
