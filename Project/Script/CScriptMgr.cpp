#include "pch.h"
#include "CScriptMgr.h"

#include <Engine/CScript.h>

//�� �Ʒ��� ��� ��ũ��Ʈ�� include

CScriptMgr::CScriptMgr() {}

CScriptMgr::~CScriptMgr()
{
	for (const auto& iter : m_umapScript)
	{
		DESTRUCTOR_DELETE(iter.second);
	}
}

void CScriptMgr::init() 
{ 
	//���⿡ CodeGenerator ���α׷����� �ڵ带 �������ش�.
}
