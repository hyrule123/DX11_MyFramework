#include "pch.h"
#include "CRes.h"

#include "jsoncpp.h"

#include "strKey_Default.h"

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

bool CRes::Save(const std::filesystem::path& _fileName)
{
	//엔진 내부에서 생성되는 기본 리소스일경우 저장 하지 않음
	if (m_bEngineDefaultRes)
		return true;

	//ResType을 인덱스로 써서 상대경로를 받아올 수 있다.
	std::filesystem::path FilePath = GETRESPATH;
	FilePath /= _fileName;

	const std::filesystem::path& parentDir = FilePath.parent_path();
	if (false == std::filesystem::exists(parentDir))
	{
		try { std::filesystem::create_directories(parentDir); }
		catch (const std::filesystem::filesystem_error& _err)
		{
			ERROR_MESSAGE(_err.what());
			DEBUG_BREAK;
			return false;
		}
	}

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
	(*_pJson)[string(RES_INFO::JSON_KEY::strKey)] = m_strKey;
	//(*_pJson)[string(RES_INFO::JSON_KEY::eRES_TYPE)] = (int)m_eResType;

	return true;
}

bool CRes::Load(const std::filesystem::path& _fileName)
{
	//ResType을 인덱스로 써서 상대경로를 받아올 수 있다.
	std::filesystem::path FilePath = GETRESPATH;
	FilePath /= _fileName;

	//키값이 지정되지 않았을 경우 파일명을 키값으로 사용
	if(GetKey().empty())
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


