#include "pch.h"
#include "CScript.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"


CScript::CScript()
	: m_strKey()
	, m_pHolder()
	, m_bDisable()
{
}

CScript::CScript(const CScript& _other)
	: CEntity(_other)
	, m_strKey(_other.m_strKey)
	, m_pHolder()
	, m_bDisable()
{
}

CScript::~CScript()
{
}
