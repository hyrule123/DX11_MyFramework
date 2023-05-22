#include "pch.h"
#include "CScript.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"


CScript::CScript()
	: m_pHolder()
{
}

CScript::CScript(const CScript& _other)
	: CEntity(_other)
	, m_pHolder()
{
}

CScript::~CScript()
{
}
