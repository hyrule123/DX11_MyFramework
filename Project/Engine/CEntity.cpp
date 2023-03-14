#include "pch.h"
#include "CEntity.h"

#include "jsoncpp.h"

namespace JSONKEY_CEntity
{
	JSON_KEY(strName);
}



UINT CEntity::g_iNextID = 0;

CEntity::CEntity()
	: m_iID(g_iNextID++)
{
}

CEntity::CEntity(const string& _strName)
	: m_strName(_strName)
	, m_iID(g_iNextID++)
{
}


CEntity::CEntity(const CEntity& _other)
	: m_strName(_other.m_strName)
	, m_iID(g_iNextID++)
{
}
 
CEntity::~CEntity()
{
}

bool CEntity::SaveJson(Json::Value* _pJson)
{
	if (nullptr == _pJson)
		return false;

	(*_pJson)[JSONKEY_CEntity::strName] = m_strName;

	return true;
}

bool CEntity::LoadJson(Json::Value* _pJson)
{
	if (nullptr == _pJson)
		return false;

	m_strName = (*_pJson)[JSONKEY_CEntity::strName].asString();

	return true;
}
