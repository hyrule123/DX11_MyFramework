#include "pch.h"
#include "CEntity.h"

#include "jsoncpp.h"

#include "strKey_Default.h"

namespace JSON_KEY
{
	JSONKEY(strName);
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

	Json::Value& jVal = *_pJson;

	jVal[string(JSON_KEY::strName)] = m_strName;

	return true;
}

bool CEntity::LoadJson(Json::Value* _pJson)
{
	if (nullptr == _pJson)
		return false;

	const Json::Value& jVal = *_pJson;

	m_strName = jVal[string(JSON_KEY::strName)].asString();

	return true;
}
