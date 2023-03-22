#include "pch.h"
#include "CRes.h"

#include "jsoncpp.h"

#include "strKeyDefault.h"

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

bool CRes::Load(const std::filesystem::path& _fileName)
{
	//ResType을 인덱스로 써서 상대경로를 받아올 수 있다.
	std::filesystem::path FilePath = RELATIVE_PATH::RES_ARR[(int)GetResType()];
	FilePath /= _fileName;

	std::ifstream inFile(FilePath);
	if (inFile.is_open())
	{
		Json::Value LoadVal;
		inFile >> LoadVal;
		inFile.close();

		return LoadJson(&LoadVal);
	}

	return false;
}

bool CRes::LoadJson(Json::Value* _pJson)
{
	if (nullptr == _pJson)
		return false;
	else if (false == CEntity::LoadJson(_pJson))
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
	//ResType을 인덱스로 써서 상대경로를 받아올 수 있다.
	std::filesystem::path FilePath = RELATIVE_PATH::RES_ARR[(int)GetResType()];
	FilePath /= _fileName;

	std::ofstream outFile(FilePath);
	if (outFile.is_open())
	{
		Json::Value SaveVal;

		bool Suc = SaveJson(&SaveVal);
		if (true == Suc)
		{
			outFile << SaveVal;
		}
		outFile.close();

		return Suc;
	}

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
