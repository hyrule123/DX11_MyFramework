#include "pch.h"
#include "cScript.h"
#include "cTransform.h"
#include "cMeshRenderer.h"
#include "cTimeMgr.h"
#include "cKeyMgr.h"


cScript::cScript(const string_view _strKey)
	: m_strKey(_strKey)
	, m_pHolder()
	, m_bDisable()
{
}

cScript::cScript(const cScript& _other)
	: cEntity(_other)
	, m_strKey(_other.m_strKey)
	, m_pHolder()
	, m_bDisable()
{
}

cScript::~cScript()
{
}
