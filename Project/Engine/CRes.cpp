#include "pch.h"
#include "CRes.h"

#include "jsoncpp.h"

#include "strKeyDefault.h"

#include "CPathMgr.h"


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



bool CRes::Load(const std::filesystem::path& _fileName)
{
	//ResType을 인덱스로 써서 상대경로를 받아올 수 있다.
	std::filesystem::path FilePath = GETRESPATH;
	FilePath /= _fileName;

	SetKey(_fileName.string());

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

	const Json::Value& jVal = *_pJson;

	//m_eResType = (eRES_TYPE)(*_pJson)[string(RES_INFO::JSON_KEY::eRES_TYPE].asInt();
	//if (jVal.isMember(string(RES_INFO::JSON_KEY::strKey)))

	if (jVal.isMember(string(RES_INFO::JSON_KEY::eRES_TYPE)))
	{
		if ((eRES_TYPE)jVal[string(RES_INFO::JSON_KEY::eRES_TYPE)].asInt() != m_eResType)
		{
			MessageBoxA(nullptr, "Resource Type Seems not matching.\nMay cause error.", nullptr, MB_OK);
		}
		//m_eResType = (eRES_TYPE)jVal[string(RES_INFO::JSON_KEY::eRES_TYPE)].asInt();
	}
		
	
	return true;
}

bool CRes::Save(const std::filesystem::path& _fileName)
{
	//ResType을 인덱스로 써서 상대경로를 받아올 수 있다.
	std::filesystem::path FilePath = GETRESPATH;
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

	(*_pJson)[string(RES_INFO::JSON_KEY::eRES_TYPE)] = (int)m_eResType;
	//(*_pJson)[string(RES_INFO::JSON_KEY::strKey)] = m_strKey;
	(*_pJson)[string(RES_INFO::JSON_KEY::eRES_TYPE)] = (int)m_eResType;

	return true;
}
