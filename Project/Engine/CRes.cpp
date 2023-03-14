#include "pch.h"
#include "CRes.h"

#include "jsoncpp.h"

namespace JSONKEY_CRes
{
	//동적할당될 때 정해지는 값이므로 굳이 생성할 필요 없을듯?
	//JSON_KEY(eRES_TYPE);
	JSON_KEY(strKey);
	JSON_KEY(RelativePath);
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
	, m_RelativePath(_other.m_RelativePath)
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
	(*_pJson)[JSONKEY_CRes::strKey] = m_strKey;
	(*_pJson)[JSONKEY_CRes::RelativePath] = m_RelativePath.string();


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
	(*_pJson)[JSONKEY_CRes::RelativePath] = m_RelativePath.string();

	return true;
}
