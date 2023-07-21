#include "pch.h"
#include "cPrefab.h"

#include "cGameObject.h"

#include "cPathMgr.h"
#include "jsoncpp.h"

#include "define.h"

namespace JsonKey_cPrefab
{

}

cPrefab::cPrefab()
	: IRes(eRES_TYPE::PREFAB)
	, m_pPrefab()
	, m_bSaveMode()
{
}

cPrefab::~cPrefab()
{
	////저장모드가 아닐 경우에만 삭제
	if(false == m_bSaveMode)
		SAFE_DELETE(m_pPrefab);
}

void cPrefab::RegisterPrefab(cGameObject* _pPrefab, bool _bIsSaveMode)
{
	assert(_pPrefab);

	m_pPrefab = _pPrefab;
	m_bSaveMode = _bIsSaveMode;
}

cGameObject* cPrefab::Instantiate()
{
	assert(m_pPrefab != nullptr);

	return m_pPrefab->Clone();
}

bool cPrefab::Save(const std::filesystem::path& _fileName)
{
	SetKey(_fileName.string());

	const std::filesystem::path& prefabPath = cPathMgr::GetInst()->GetPathRel_Resource(eRES_TYPE::PREFAB);
	std::filesystem::path filePath = prefabPath / _fileName;
	filePath += RES_INFO::PREFAB::Ext;

	std::ofstream saveFile(filePath);
	if (saveFile.is_open())
	{
		Json::Value jsonSave;

		if (false == SaveJson(&jsonSave))
			return false;

		saveFile << jsonSave;

		saveFile.close();

		return true;
	}

	return false;
}

bool cPrefab::Load(const std::filesystem::path& _fileName)
{
	const std::filesystem::path& prefabPath = cPathMgr::GetInst()->GetPathRel_Resource(eRES_TYPE::PREFAB);
	std::filesystem::path filePath = prefabPath / _fileName;
	filePath += RES_INFO::PREFAB::Ext;

	std::ifstream loadFile(filePath);
	if (loadFile.is_open())
	{
		Json::Value jsonLoad;

		loadFile >> jsonLoad;

		loadFile.close();

		if (false == LoadJson(&jsonLoad))
			return false;

		

		return true;
	}

	return false;
}

bool cPrefab::SaveJson(Json::Value* _pJVal)
{
	if (nullptr == _pJVal || nullptr == m_pPrefab)
		return false;
	else if (false == IRes::SaveJson(_pJVal))
		return false;

	if (m_pPrefab->GetKey().empty())
	{
		ERROR_MESSAGE("No Key!");
		return false;
	}

	Json::Value& jVal = *_pJVal;

	string strKey(GetKey());
	jVal[strKey] = Json::Value(Json::ValueType::objectValue);

	Json::Value& jsonGameObject = jVal[strKey];
	if (false == m_pPrefab->SaveJson(&jsonGameObject))
		return false;

	return true;
}

bool cPrefab::LoadJson(Json::Value* _pJVal)
{
	if (nullptr == _pJVal)
		return false;
	else if (false == IRes::LoadJson(_pJVal))
		return false;

	Json::Value& jVal = *_pJVal;

	if (m_pPrefab)
		SAFE_DELETE(m_pPrefab);

	m_pPrefab = new cGameObject;

	string strKey(GetKey());
	if (jVal.isMember(strKey))
	{
		Json::Value& jValGameObject = jVal[strKey];
		if (false == m_pPrefab->LoadJson(&jValGameObject))
		{
			SAFE_DELETE(m_pPrefab);
			return false;
		}
	}

	return true;
}

