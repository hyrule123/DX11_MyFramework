#include "pch.h"
#include "cEntity.h"

#include "jsoncpp.h"

#include "strKey_Default.h"

namespace JSON_KEY
{
	STRKEY_DECLARE(m_strKey);
}

UINT cEntity::g_iNextID = 0;

cEntity::cEntity()
	: m_iID(g_iNextID++)
{
}

cEntity::cEntity(const string_view _strKey)
	: m_strKey(_strKey)
	, m_iID(g_iNextID++)
{
}


cEntity::cEntity(const cEntity& _other)
	: m_strKey(_other.m_strKey)
	, m_iID(g_iNextID++)
{
}
 
cEntity::~cEntity()
{
}

bool cEntity::SaveJson(Json::Value* _pJson)
{
	if (nullptr == _pJson)
		return false;

	Json::Value& jVal = *_pJson;

	jVal[JSON_KEY::m_strKey] = m_strKey;

	return true;
}

bool cEntity::LoadJson(Json::Value* _pJson)
{
	if (nullptr == _pJson)
		return false;

	const Json::Value& jVal = *_pJson;

	if(jVal.isMember(JSON_KEY::m_strKey))
		m_strKey = jVal[JSON_KEY::m_strKey].asString();

	return true;
}
