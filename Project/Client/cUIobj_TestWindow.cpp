#include "pch.h"
#include "cUIobj_TestWindow.h"

#include "cUI_Button.h"
#include "cUI_ComboBox.h"

#include <Engine/strKey_Default.h>


#include <Engine/cPathMgr.h>
#include <Engine/cResMgr.h>
#include <Engine/cGameObject.h>
#include <Engine/cTransform.h>

#include <Engine/cCom_Renderer_Default.h>
#include <Engine/strKey_Default.h>
#include <Engine/cCom_Renderer_TilemapComplete.h>
#include <Script/cCom_Renderer_TilemapSC.h>

#include <Script/strKey_Shader.h>
#include <Script/strKey_Component.h>

#include <Engine/EventDispatcher.h>
#include <Engine/cLevelMgr.h>
#include <Engine/cLevel.h>

#include <Engine/RandGen.h>

#include <Script/define_SC.h>

//카메라 위치 받아오기 용
#include <Engine/cRenderMgr.h>
#include <Engine/cCom_Camera.h>


cUIobj_TestWindow::cUIobj_TestWindow()
	: cUI_BasicWindow("TestWindow")
	, m_pMapObj()
	, m_TilemapRenderer()
{
	SetSaveEnable(true);
}

cUIobj_TestWindow::~cUIobj_TestWindow()
{
}

void cUIobj_TestWindow::init()
{
    cUI_Button* pButton = (cUI_Button*)FindChildUIByName("TestButton");

	cUI_ComboBox* pCombo = new cUI_ComboBox("MapChange");

	pCombo->SetWidth(200.f);
	pCombo->SetLeftLabel(true);
	pCombo->SetLeftLabelWidth(100.f);

	std::filesystem::directory_iterator MapDirIter;
	try 
	{
		std::filesystem::path MapPath = cPathMgr::GetInst()->GetPathRel_Content();
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

	pCombo->AddClickCallback(std::bind(&cUIobj_TestWindow::LoadMapData, this, std::placeholders::_1), eCALLBACK_TYPE::ONSELECT);

	AddChildUI(pCombo);
}

void cUIobj_TestWindow::render_update()
{
	if (nullptr == m_pMapObj)
		FindTestObj();

	m_TilemapRenderer = m_pMapObj->GetComponent<cCom_Renderer_TilemapSC>();
	if (nullptr == m_TilemapRenderer)
		return;
	if (ImGui::Button("Map Debug: None"))
	{
		m_TilemapRenderer->SetTilemapDebugMode(eTILEMAP_DEBUGMODE::NONE);
	}

	if (ImGui::Button("Map Debug: MegaTile"))
	{
		m_TilemapRenderer->SetTilemapDebugMode(eTILEMAP_DEBUGMODE::MEGATILE);
	}

	if (ImGui::Button("Map Debug: MiniTile"))
	{
		m_TilemapRenderer->SetTilemapDebugMode(eTILEMAP_DEBUGMODE::MINITILE);
	}

	if (ImGui::Button("Spawn Marines"))
	{
		cCom_Camera* pCam = cRenderMgr::GetInst()->GetCurCamera();
		const Vec2& CamPos = pCam->GetOwner()->Transform().GetRelativePos().XY();

		Ptr<cPrefab> Marine = cResMgr::GetInst()->Load<cPrefab>(SC::GetUnitName(SC::eUNIT_ID::TERRAN_MARINE));

		for (int i = 0; i < 10; ++i)
		{
			Vec2 RandVal = Vec2(640.f, 480.f) * 0.5f;
			RandVal.x = RandGen::GetRand<float>(-RandVal.x, RandVal.x);
			RandVal.y = RandGen::GetRand<float>(-RandVal.y, RandVal.y);
			RandVal += CamPos;

			cGameObject* pObj = EventDispatcher::SpawnPrefab2D(Marine, RandVal);

			//pObj->ScriptHolder()->Transition(SC::FSM::ATTACK);
		}
	}

	//if (ImGui::Button("Save GameObject"))
	//{
	//	std::filesystem::path TestSave = "ParentTest.json";
	//	cPrefab* pPrefab = new cPrefab;

	//	pPrefab->SetKey(TestSave.string());

	//	cLevel* level = cLevelMgr::GetInst()->GetCurLevel();

	//	cGameObject* pObj = level->FindObjectByName("ParentTest");

	//	pPrefab->RegisterPrefab(pObj, true);

	//	pPrefab->SetKey(TestSave.string());

	//	pPrefab->Save(TestSave);

	//	delete pPrefab;
	//}

	//if (ImGui::Button("Load GameObject"))
	//{
	//	Ptr<cPrefab> prefab = cResMgr::GetInst()->Load<cPrefab>("ParentTest.json");

	//	cGameObject* pObj = prefab->Instantiate();

	//	EventDispatcher::SpawnGameObject(pObj, Vec3(RandGen::GetRand(-640.f, 640.f), RandGen::GetRand(-320.f, 320.f), 0.f), 0);
	//}

}

void cUIobj_TestWindow::LoadMapData(const tComboItem& _tCombo)
{
	if (nullptr == m_TilemapRenderer)
		return;

	m_TilemapRenderer->LoadMap(_tCombo.strName);
}

void cUIobj_TestWindow::FindTestObj()
{
	m_pMapObj = cLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(SC::strKey_PREFAB::MAPOBJ);

	assert(m_pMapObj);
}

void cUIobj_TestWindow::ChangeDebugMode(int _iMode)
{

}
