#include "pch.h"
#include "CScript.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"



CScript::CScript(const string& _strKey)
	: m_strKey(_strKey)
	, m_pHolder()
{
	//key값이 비어있을 경우 에러를 발생
	assert(false == _strKey.empty());
}

CScript::CScript(const CScript& _other)
	: CEntity(_other)
	, m_strKey(_other.m_strKey)
	, m_pHolder()
{
}

CScript::~CScript()
{
}
