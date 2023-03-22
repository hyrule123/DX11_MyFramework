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


    AddChildUI(new CUI_Button("TestButton"));

    CUI_Button* pButton = (CUI_Button*)FindChildUIByName("TestButton");

	CUI_ComboBox* pCombo = new CUI_ComboBox("MapChange");

	pCombo->SetWidth(200.f);
	pCombo->SetLeftLabel(true);
	pCombo->SetLeftLabelWidth(100.f);

	std::filesystem::directory_iterator MapDirIter;
	try 
	{
		MapDirIter = std::filesystem::directory_iterator(RELATIVE_PATH::SCMAP::A);
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

			//확장자가 일치하지 않을 경우 읽어오지 않는다.
			for (int i = 0; i < (int)RES_INFO::SCMAP::eSCMAP_TYPE::END; ++i)
			{
				if (RES_INFO::SCMAP::arrExtension[i] != extension)
					continue;
			}
		}
		

		tComboItem item{ MapDirIter->path().filename().string(), };
		pCombo->AddItem(item);
	}

	pCombo->AddClickCallback(std::bind(&CUIobj_TestWindow::LoadMapData, this, std::placeholders::_1), eCALLBACK_TYPE::ONSELECT);

	AddChildUI(pCombo);
}

void CUIobj_TestWindow::render_update()
{

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
	m_pTestObj->AddComponent(new CTransform);
	
	CTilemapComplete* TilemapComp = new CTilemapComplete;
	m_pTestObj->AddComponent(TilemapComp);

	::SpawnGameObject(m_pTestObj, Vec3(0.f, 0.f, 1000.f), 0);
}
