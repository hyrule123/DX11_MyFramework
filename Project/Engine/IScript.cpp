#include "pch.h"
#include "IScript.h"
#include "cCom_Transform.h"
#include "cCom_Renderer_Basic.h"
#include "cTimeMgr.h"
#include "cKeyMgr.h"


IScript::IScript(const string_view _strKey)
	: m_strKey(_strKey)
	, m_pHolder()
	, m_bDisable()
{
}

IScript::IScript(const IScript& _other)
	: IEntity(_other)
	, m_strKey(_other.m_strKey)
	, m_pHolder()
	, m_bDisable()
{
}

IScript::~IScript()
{
}
