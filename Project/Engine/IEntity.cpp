#include "pch.h"
#include "IEntity.h"

#include "jsoncpp.h"

#include "strKey_Default.h"

#include "cComMgr.h"

UINT IEntity::g_iNextID = 0;

IEntity::IEntity()
	: m_iID(g_iNextID++)
{
}

IEntity::IEntity(const string_view _strKey)
	: m_strKey(_strKey)
	, m_iID(g_iNextID++)
{
}


IEntity::IEntity(const IEntity& _other)
	: m_strKey(_other.m_strKey)
	, m_iID(g_iNextID++)
{
}
 
IEntity::~IEntity()
{
}

bool IEntity::SaveJson(Json::Value* _pJson)
{
	if (nullptr == _pJson)
		return false;

	Json::Value& jVal = *_pJson;

	if (m_strKey.empty())
	{
		ERROR_MESSAGE("Can't save without String Key!!");
		return false;
	}

	jVal[JsonKey_IEntity::m_strKey] = m_strKey;

	return true;
}

bool IEntity::LoadJson(Json::Value* _pJson)
{
	if (nullptr == _pJson)
		return false;

	const Json::Value& jVal = *_pJson;

	if (jVal.isMember(JsonKey_IEntity::m_strKey))
	{
		m_strKey = jVal[JsonKey_IEntity::m_strKey].asString();
	}
		

	return true;
}
