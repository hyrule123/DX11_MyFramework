#include "pch.h"
#include "CUIobj_TestWindow.h"

#include "CUI_Button.h"
#include "CUI_ComboBox.h"

#include <Engine/strKey_Default.h>

#include <Engine/CPathMgr.h>
#include <Engine/CResMgr.h>
#include <Engine/CCS_SCMapLoader.h>
#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>
#include <Engine/CMeshRender.h>
#include <Engine/strKey_Default.h>
#include <Engine/CTilemap_SC.h>
#include <Script/strKey_Shader.h>

#include <Engine/EventDispatcher.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include <Engine/CRandMgr.h>

#include <Script/define_SC.h>

//카메라 위치 받아오기 용
#include <Engine/CRenderMgr.h>
#include <Engine/CCamera.h>

CUIobj_TestWindow::CUIobj_TestWindow()
	: CUI_BasicWindow("TestWindow")
	, m_pMapObj()
{
	SetSaveEnable(true);
}

CUIobj_TestWindow::~CUIobj_TestWindow()
{
}

void CUIobj_TestWindow::init()
{
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
	if (nullptr == m_pMapObj)
		FindTestObj();

	CTilemap_SC* pTilemap = static_cast<CTilemap_SC*>(m_pMapObj->Tilemap());
	if (nullptr == pTilemap)
		return;
	if (ImGui::Button("Map Debug: None"))
	{
		pTilemap->SetTilemapDebugMode(eTILEMAP_DEBUGMODE::NONE);
	}

	if (ImGui::Button("Map Debug: MegaTile"))
	{
		pTilemap->SetTilemapDebugMode(eTILEMAP_DEBUGMODE::MEGATILE);
	}

	if (ImGui::Button("Map Debug: MiniTile"))
	{
		pTilemap->SetTilemapDebugMode(eTILEMAP_DEBUGMODE::MINITILE);
	}

	if (ImGui::Button("Spawn Marines"))
	{
		CCamera* pCam = CRenderMgr::GetInst()->GetCurCamera();
		const Vec2& CamPos = pCam->GetOwner()->Transform().GetRelativePos().XY();

		Ptr<CPrefab> Marine = CResMgr::GetInst()->Load<CPrefab>(SC::GetUnitName(SC::eUNIT_ID::TERRAN_MARINE));

		for (int i = 0; i < 10; ++i)
		{
			Vec2 RandVal = Vec2(640.f, 480.f) * 0.5f;
			RandVal.x = CRandMgr::GetInst()->GetRand<float>(-RandVal.x, RandVal.x);
			RandVal.y = CRandMgr::GetInst()->GetRand<float>(-RandVal.y, RandVal.y);
			RandVal += CamPos;

			EventDispatcher::SpawnPrefab2D(Marine, RandVal);
		}
	}

	//if (ImGui::Button("Save GameObject"))
	//{
	//	std::filesystem::path TestSave = "ParentTest.json";
	//	CPrefab* pPrefab = new CPrefab;

	//	pPrefab->SetKey(TestSave.string());

	//	CLevel* level = CLevelMgr::GetInst()->GetCurLevel();

	//	CGameObject* pObj = level->FindObjectByName("ParentTest");

	//	pPrefab->RegisterPrefab(pObj, true);

	//	pPrefab->SetKey(TestSave.string());

	//	pPrefab->Save(TestSave);

	//	delete pPrefab;
	//}

	//if (ImGui::Button("Load GameObject"))
	//{
	//	Ptr<CPrefab> prefab = CResMgr::GetInst()->Load<CPrefab>("ParentTest.json");

	//	CGameObject* pObj = prefab->Instantiate();

	//	EventDispatcher::SpawnGameObject(pObj, Vec3(CRandMgr::GetInst()->GetRand(-640.f, 640.f), CRandMgr::GetInst()->GetRand(-320.f, 320.f), 0.f), 0);
	//}

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
			pMapLoader = pResMgr->FindRes<CComputeShader>(string(strKey_SHADER::COMPUTE::SCMAPLOADER));
			if (nullptr == pMapLoader)
				throw std::runtime_error("Cannot find Map Loader!!");
			
			bool bLoaded = false;
			CTilemap* pTilemap = m_pMapObj->Tilemap();
			if (eTILE_TYPE::COMPLETE == pTilemap->GetTileType())
			{
				bLoaded = (static_cast<CTilemap_SC*>(pTilemap))->LoadMap(_tCombo.strName);
			}

			if (false == bLoaded)
			{
				DEBUG_BREAK;
				throw std::runtime_error("Map Load Failed!!");
			}
				
		}
		catch (const std::exception& error)
		{
			MessageBoxA(nullptr, error.what(), NULL, MB_OK);
		}

	}
}

void CUIobj_TestWindow::FindTestObj()
{
	m_pMapObj = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(SC::strKey_PREFAB::MAPOBJ);

	assert(m_pMapObj);
}

void CUIobj_TestWindow::ChangeDebugMode(int _iMode)
{

}
