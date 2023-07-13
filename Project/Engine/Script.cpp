#include "pch.h"
#include "Script.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "TimeMgr.h"
#include "KeyMgr.h"


Script::Script(const string_view _strKey)
	: m_strKey(_strKey)
	, m_pHolder()
	, m_bDisable()
{
}

Script::Script(const Script& _other)
	: Entity(_other)
	, m_strKey(_other.m_strKey)
	, m_pHolder()
	, m_bDisable()
{
}

Script::~Script()
{
}
