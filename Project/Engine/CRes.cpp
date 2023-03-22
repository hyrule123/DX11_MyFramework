#include "pch.h"
#include "CRes.h"

#include "jsoncpp.h"

namespace JSONKEY_CRes
{
	JSON_KEY(eRES_TYPE);
	JSON_KEY(strKey);
	//JSON_KEY(RelativePath);
}


CRes::CRes(eRES_TYPE _type)
	: m_eResType(_type)
	, m_iRefCount(0)
{
}

CRes::CRes(const CRes& _other)
	: m_eResType(_other.m_eResType)
	, m_iRefCount(0)
	, m_strKey(_other.m_strKey)
{
}

CRes::~CRes()
{
}

void CRes::Release()
{
	--m_iRefCount;

	if (0 == m_iRefCount)
	{
		delete this;
	}
}

bool CRes::LoadJson(Json::Value* _pJson)
{
	if (nullptr == _pJson)
		return false;

	//m_eResType = (eRES_TYPE)(*_pJson)[JSONKEY_CRes::eRES_TYPE].asInt();
	if(_pJson->isMember(JSONKEY_CRes::strKey))
		m_strKey = (*_pJson)[JSONKEY_CRes::strKey].asString();

	if (_pJson->isMember(JSONKEY_CRes::eRES_TYPE))
		m_eResType = (eRES_TYPE)(*_pJson)[JSONKEY_CRes::eRES_TYPE].asInt();
	
	return true;
}

bool CRes::Save(const std::filesystem::path& _fileName)
{
	return false;
}

bool CRes::SaveJson(Json::Value* _pJson)
{
	if (nullptr == _pJson)
		return false;

	else if (false == CEntity::SaveJson(_pJson))
		return false;

	//(*_pJson)[JSONKEY_CRes::eRES_TYPE] = (int)m_eResType;
	(*_pJson)[JSONKEY_CRes::strKey] = m_strKey;
	(*_pJson)[JSONKEY_CRes::eRES_TYPE] = (int)m_eResType;

	return true;
}
