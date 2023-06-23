#include "pch.h"
#include "CMainGameLevel.h"

#include <Engine/global.h>

#include <Engine/CDevice.h>

#include <Engine/CLevel.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLayer.h>

#include <Engine/ptr.h>
#include <Engine/CTexture.h>
#include <Engine/CResMgr.h>
#include <Engine/strKey_Default.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>
#include <Engine/CCollisionMgr.h>
#include <Engine/CTimeMgr.h>

#include <Engine/CComputeShader.h>
#include <Engine/CCS_SetColor.h>
#include <Engine/CCS_SCMapLoader.h>
#include <Engine/CScriptMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CResMgr.h>
#include <Engine/CRandMgr.h>
#include <Engine/EventDispatcher.h>
#include <Engine/jsoncpp.h>

#include <Script/strKey_Script.h>
#include <Script/strKey_Texture.h>
#include <Script/strKey_Shader.h>

#include <Script/CScript_FSM_Move_Ground.h>
#include <Script/CScript_MouseCursor.h>
#include <Script/define_SC.h>

#include "ManualEdit.h"
#include "strKey_Prefab.h"

#define LAYER_2D_Z_INTERVAL 100.f

void CreateMainGame()
{
	ManualEdit::Edit();

	LoadRes(eRES_TYPE::TEXTURE);
	LoadRes(eRES_TYPE::MATERIAL);
	LoadRes(eRES_TYPE::PREFAB);

	ManualEdit::TestCreate();


	//Layer 세팅
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurLevel();

	//Layer 이름 세팅
	for (int i = 0; i < (int)SC::LAYER_INFO::idx::END; ++i)
	{
		CLayer& Layer = pLevel->GetLayer(i);

		Layer.SetName(SC::LAYER_INFO::strLayerName[i]);

		bool bYSort = false;

		switch (i)
		{
		case (int)SC::LAYER_INFO::GroundUnitMain:
		case (int)SC::LAYER_INFO::Resource:
			bYSort = true;

		default:
			break;
		}

		Layer.SetPresetZDepth_Ysort((MAX_LAYER - 1 - i) * LAYER_2D_Z_INTERVAL, bYSort);
	}

	//Layer Z 정보 세팅
	CCollisionMgr::GetInst()->AddLayerInterAction2DAll(SC::LAYER_INFO::MouseCursor);
	CCollisionMgr::GetInst()->AddLayerInteraction2D(SC::LAYER_INFO::GroundUnitMain, SC::LAYER_INFO::GroundUnitMain);
	CCollisionMgr::GetInst()->AddLayerInteraction2D(SC::LAYER_INFO::Resource, SC::LAYER_INFO::GroundUnitMain);

	CCollisionMgr::GetInst()->AddLayerInteraction2D(0, 1);

	Ptr<CMesh> CircleMesh = CResMgr::GetInst()->FindRes<CMesh>("CircleMesh");
	Ptr<CMesh> RectMesh = CResMgr::GetInst()->FindRes<CMesh>("RectMesh");

	CResMgr* pResMgr = CResMgr::GetInst();

	//Map Object
	{
		Ptr<CPrefab> pPrefab = pResMgr->Load<CPrefab>(SC::strKey_PREFAB::MAPOBJ);
		assert(nullptr != pPrefab);
		EventDispatcher::SpawnGameObject(pPrefab->Instantiate(), Vec3::Zero);
	}

	{
		// Camera	
		CGameObject* pObj = new CGameObject;
		pObj->SetName("Camera");
		CCamera* Cam = new CCamera;
		pObj->AddComponent(Cam);
		pObj->Camera()->SetCamIndex(eCAMERA_INDEX::MAIN);
		pObj->Camera()->SetProjType(ePROJ_TYPE::ORTHOGRAPHY);
		//pObj->AddComponent(new CTransform);
		pObj->AddScript(CScriptMgr::GetInst()->GetNewScript(strKey_SCRIPT::MAINCAMSC_INGAME));

		EventDispatcher::SpawnGameObject(pObj, Vec3(0.f, 0.f, 0.f), SC::LAYER_INFO::Camera);
	}
}

void LoadRes(eRES_TYPE _eResType)
{
	CResMgr* pResMgr = CResMgr::GetInst();
	//우선 테스트를 위해서 모든 리소스를 순회돌면서 로드해준다.
	//텍스처 로드
	std::filesystem::path ResPath = CPathMgr::GetInst()->GetPathRel_Resource(_eResType);

	std::filesystem::recursive_directory_iterator RIter;
	try
	{
		RIter = std::filesystem::recursive_directory_iterator(ResPath);

		for (RIter; RIter != std::filesystem::end(RIter); ++RIter)
		{
			if (true == RIter->is_directory())
				continue;
			const auto& RelativePath = std::filesystem::relative(RIter->path(), ResPath);
			pResMgr->Load(_eResType, RelativePath);
			//pResMgr->Load<CTexture>(RelativePath);
		}
	}
	catch (const std::filesystem::filesystem_error& error)
	{
		MessageBoxA(nullptr, error.what(), NULL, MB_OK);
		throw(error);
	}
}

