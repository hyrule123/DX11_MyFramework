#include "pch.h"
#include "CPrefab.h"

#include "CGameObject.h"

#include "CPathMgr.h"
#include "jsoncpp.h"

#include "define.h"

CPrefab::CPrefab()
	: CRes(eRES_TYPE::PREFAB)
	, m_pPrefab()
	, m_bSaveMode()
{
}

CPrefab::~CPrefab()
{
	////저장모드가 아닐 경우에만 삭제
	if(false == m_bSaveMode)
		SAFE_DELETE(m_pPrefab);
}

void CPrefab::RegisterPrefab(CGameObject* _pPrefab, bool _bIsSaveMode)
{
	//Prefab은 레이어에 등록되어 실행되는 용도가 아님.
	//assert(0 > _pPrefab->GetLayer());

	m_pPrefab = _pPrefab;
	m_bSaveMode = _bIsSaveMode;

	const string& strKey = GetKey();
	if (strKey.empty())
	{
		ERROR_MESSAGE("You must set string Key!!");
		DEBUG_BREAK;
		return;
	}

	if (m_pPrefab)
		m_pPrefab->SetName(GetKey());
}

CGameObject* CPrefab::Instantiate()
{
	assert(m_pPrefab != nullptr);

	return m_pPrefab->Clone();
}

bool CPrefab::Save(const std::filesystem::path& _fileName)
{
	const std::filesystem::path& prefabPath = CPathMgr::GetInst()->GetPathRel_Resource(eRES_TYPE::PREFAB);
	std::filesystem::path filePath = prefabPath / _fileName;

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

bool CPrefab::Load(const std::filesystem::path& _fileName)
{
	const std::filesystem::path& prefabPath = CPathMgr::GetInst()->GetPathRel_Resource(eRES_TYPE::PREFAB);
	std::filesystem::path filePath = prefabPath / _fileName;

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

bool CPrefab::SaveJson(Json::Value* _pJVal)
{
	if (nullptr == _pJVal || nullptr == m_pPrefab)
		return false;
	else if (false == CRes::SaveJson(_pJVal))
		return false;

	Json::Value& jVal = *_pJVal;

	string strKey = string(RES_INFO::JSON_KEY::GameObject);
	jVal[strKey] = Json::Value(Json::ValueType::objectValue);

	Json::Value& jsonGameObject = jVal[strKey];
	if (false == m_pPrefab->SaveJson(&jsonGameObject))
		return false;

	return true;
}

bool CPrefab::LoadJson(Json::Value* _pJVal)
{
	if (nullptr == _pJVal)
		return false;
	else if (false == CRes::LoadJson(_pJVal))
		return false;

	Json::Value& jVal = *_pJVal;

	if (m_pPrefab)
		SAFE_DELETE(m_pPrefab);

	m_pPrefab = new CGameObject;

	string strKey = string(RES_INFO::JSON_KEY::GameObject);
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

