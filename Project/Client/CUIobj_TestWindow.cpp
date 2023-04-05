#include "pch.h"
#include "CUIobj_TestWindow.h"

#include "CUI_Button.h"
#include "CUI_ComboBox.h"

#include <Engine/strKeyDefault.h>

#include <Engine/CPathMgr.h>
#include <Engine/CResMgr.h>
#include <Engine/CCS_SCMapLoader.h>
#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>
#include <Engine/CMeshRender.h>
#include <Engine/strKeyDefault.h>
#include <Engine/CTilemapComplete.h>
#include <Script/strKeyShader.h>

#include <Engine/EventDispatcher.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CUIobj_TestWindow::CUIobj_TestWindow()
	: CUI_BasicWindow("TestWindow")
	, m_pTestObj()
{
	SetSaveEnable(true);
}

CUIobj_TestWindow::~CUIobj_TestWindow()
{
}

void CUIobj_TestWindow::init()
{
	CreateTestObj();


    CUI_Button* pButton = (CUI_Button*)FindChildUIByName("TestButton");

	CUI_ComboBox* pCombo = new CUI_ComboBox("MapChange");

	pCombo->SetWidth(200.f);
	pCombo->SetLeftLabel(true);
	pCombo->SetLeftLabelWidth(100.f);

	std::filesystem::directory_iterator MapDirIter;
	try 
	{
		std::filesystem::path MapPath = CPathMgr::GetInst()->GetPathRel_Content();
		MapPath /= DIRECTORY_NAME::SCMAP;
		MapDirIter = std::filesystem::directory_iterator(MapPath);
	}
	catch(const std::filesystem::filesystem_error& error)
	{
		MessageBoxA(nullptr, error.what(), NULL, MB_OK);
		assert(false);
	}

	
	for (MapDirIter; false == MapDirIter._At_end(); ++MapDirIter)
	{
		if (true == MapDirIter->is_directory())
			continue;
		else
		{
			string extension = MapDirIter->path().extension().string();
			std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

			bool bContinue = true;
			//확장자가 일치하는지 확인한다.
			for (int i = 0; i < (int)RES_INFO::SCMAP::Ext::idx::END; ++i)
			{
				if (RES_INFO::SCMAP::Ext::arr[i] == extension)
				{
					bContinue = false;
					break;
				}
			}
			if (bContinue)
				continue;
		}
		

		tComboItem item{ MapDirIter->path().filename().string(), };
		pCombo->AddItem(item);
	}

	pCombo->AddClickCallback(std::bind(&CUIobj_TestWindow::LoadMapData, this, std::placeholders::_1), eCALLBACK_TYPE::ONSELECT);

	AddChildUI(pCombo);
}

void CUIobj_TestWindow::render_update()
{
	if (ImGui::Button("Save GameObject"))
	{
		std::filesystem::path TestSave = "TestSave.json";
		CPrefab* pPrefab = new CPrefab;

		pPrefab->SetKey(TestSave.string());

		CLevel* level = CLevelMgr::GetInst()->GetCurLevel();

		CGameObject* pObj = level->FindObjectByName("TestObj");

		pPrefab->RegisterPrefab(pObj, true);

		pPrefab->SetKey(TestSave.string());

		pPrefab->Save(TestSave);

		delete pPrefab;
	}

	if (ImGui::Button("Load GameObjet"))
	{
		Ptr<CPrefab> prefab = CResMgr::GetInst()->Load<CPrefab>("TestSave.json");

		CGameObject* pObj = prefab->Instantiate();

		EventDispatcher::SpawnGameObject(pObj, Vec3(0.f, 0.f, 0.f), 0);
	}

}

void CUIobj_TestWindow::LoadMapData(const tComboItem& _tCombo)
{
	CResMgr* pResMgr = CResMgr::GetInst();

	//우선 맵이 이미 로드되었는지 확인한다.
	Ptr<CTexture> pTex = pResMgr->FindRes<CTexture>(_tCombo.strName);

	//맵이 없을 경우 새로 로드
	if (nullptr == pTex)
	{
		try
		{
			Ptr<CCS_SCMapLoader> pMapLoader;
			pMapLoader = pResMgr->FindRes<CComputeShader>(string(RES_SHADER::COMPUTE::SCMAPLOADER));
			if (nullptr == pMapLoader)
				throw std::runtime_error("Cannot find Map Loader!!");
			
			bool bLoaded = false;
			CTilemap* pTilemap = m_pTestObj->Tilemap();
			if (eTILE_TYPE::COMPLETE == pTilemap->GetTileType())
			{
				bLoaded = (static_cast<CTilemapComplete*>(pTilemap))->LoadMap(_tCombo.strName);
			}

			if (false == bLoaded)
				throw std::runtime_error("Map Load Failed!!");

			
		}
		catch (const std::exception& error)
		{
			MessageBoxA(nullptr, error.what(), NULL, MB_OK);
		}

	}
}

void CUIobj_TestWindow::CreateTestObj()
{
	//Ptr<CTexture> MapTex = pMapLoader->GetMap();

	m_pTestObj = new CGameObject;

	m_pTestObj->SetName("MapObj");
	
	CTilemapComplete* TilemapComp = new CTilemapComplete;
	m_pTestObj->AddComponent(TilemapComp);

	EventDispatcher::SpawnGameObject(m_pTestObj, Vec3(0.f, 0.f, 1000.f), 0);
}
