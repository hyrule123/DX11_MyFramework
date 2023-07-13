#include "pch.h"
#include "cRes.h"

#include "jsoncpp.h"

#include "strKey_Default.h"

#include "cPathMgr.h"


cRes::cRes(eRES_TYPE _type)
	: m_eResType(_type)
	, m_iRefCount(0)
	, m_bEngineDefaultRes()
{
}

cRes::cRes(const cRes& _other)
	: cEntity(_other)
	, m_eResType(_other.m_eResType)
	, m_iRefCount(0)
	, m_bEngineDefaultRes(_other.m_bEngineDefaultRes)
{
}

cRes::~cRes()
{
}

bool cRes::Save(const std::filesystem::path& _fileName)
{
	//엔진 내부에서 생성되는 기본 리소스일경우 저장 하지 않음
	if (m_bEngineDefaultRes)
		return false;

	//ResType을 인덱스로 써서 상대경로를 받아올 수 있다.
	std::filesystem::path FilePath = GETRESPATH;
	FilePath /= _fileName;

	//해당 디렉토리가 존재하는지 확인 후 존재하지 않을 경우 디렉토리를 생성한다.
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

bool cRes::SaveJson(Json::Value* _pJson)
{
	if (nullptr == _pJson)
		return false;

	else if (false == cEntity::SaveJson(_pJson))
		return false;

	Json::Value& _jVal = *_pJson;

	_jVal[JsonKey_Res::m_eResType] = (int)m_eResType;

	return true;
}

bool cRes::Load(const std::filesystem::path& _fileName)
{
	//ResType을 인덱스로 써서 상대경로를 받아올 수 있다.
	std::filesystem::path FilePath = GETRESPATH;
	FilePath /= _fileName;

	////키값이 지정되지 않았을 경우 파일명을 키값으로 사용
	//if(GetKey().empty())
	//	SetKey(_fileName.string());

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

bool cRes::LoadJson(Json::Value* _pJson)
{
	if (nullptr == _pJson)
		return false;
	else if (false == cEntity::LoadJson(_pJson))
		return false;

	const Json::Value& jVal = *_pJson;

	if (jVal.isMember(JsonKey_Res::m_eResType))
	{
		if ((eRES_TYPE)jVal[JsonKey_Res::m_eResType].asInt() != m_eResType)
		{
			MessageBoxA(nullptr, "Resource Type Seems not matching.\nMay cause error.", nullptr, MB_OK);
		}
	}
	
	return true;
}


