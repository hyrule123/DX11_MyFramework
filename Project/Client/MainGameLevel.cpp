#include "pch.h"
#include "MainGameLevel.h"

#include <Engine/global.h>

#include <Engine/cDevice.h>

#include <Engine/cLevel.h>
#include <Engine/cLevelMgr.h>
#include <Engine/cLayer.h>

#include <Engine/ptr.h>
#include <Engine/cTexture.h>
#include <Engine/cResMgr.h>
#include <Engine/strKey_Default.h>
#include <Engine/cGameObject.h>
#include <Engine/components.h>
#include <Engine/cCollisionMgr.h>
#include <Engine/CTimeMgr.h>

#include <Engine/cComputeShader.h>


#include <Engine/CTimeMgr.h>
#include <Engine/cResMgr.h>
#include <Engine/RandGen.h>
#include <Engine/EventDispatcher.h>
#include <Engine/jsoncpp.h>

#include <Script/strKey_Component.h>
#include <Script/strKey_Texture.h>
#include <Script/strKey_Shader.h>


//#include <Script/CScript_FSM_Move_Ground.h>
#include <Script/CScript_MouseCursor.h>
#include <Script/define_SC.h>

#include <Engine/cComMgr.h>
#include <Engine/cCom_Camera.h>

#include <Script/strKey_Component.h>

#include "ManualEdit.h"
#include "strKey_Prefab.h"

#define LAYER_2D_Z_INTERVAL 100.f

void CreateMainGame()
{
	ManualEdit::Edit();

	//LoadRes(eRES_TYPE::TEXTURE);
	//LoadRes(eRES_TYPE::MATERIAL);
	//LoadRes(eRES_TYPE::PREFAB);

	ManualEdit::TestCreate();


	//Layer 세팅
	cLevel* pLevel = cLevelMgr::GetInst()->GetCurLevel();

	//Layer 이름 세팅
	for (int i = 0; i < (int)SC::LAYER_INFO::idx::END; ++i)
	{
		cLayer& Layer = pLevel->GetLayer(i);

		Layer.SetKey(SC::LAYER_INFO::strLayerName[i]);

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
	cCollisionMgr::GetInst()->AddLayerInterAction2DAll(SC::LAYER_INFO::MouseCursor);
	cCollisionMgr::GetInst()->AddLayerInteraction2D(SC::LAYER_INFO::GroundUnitMain, SC::LAYER_INFO::GroundUnitMain);
	cCollisionMgr::GetInst()->AddLayerInteraction2D(SC::LAYER_INFO::Resource, SC::LAYER_INFO::GroundUnitMain);

	cCollisionMgr::GetInst()->AddLayerInteraction2D(0, 1);

	Ptr<cMesh> CircleMesh = cResMgr::GetInst()->FindRes<cMesh>("CircleMesh");
	Ptr<cMesh> RectMesh = cResMgr::GetInst()->FindRes<cMesh>("RectMesh");

	cResMgr* pResMgr = cResMgr::GetInst();

	//Map Object
	{
		Ptr<cPrefab> pPrefab = pResMgr->Load<cPrefab>(SC::strKey_PREFAB::MAPOBJ);
		assert(nullptr != pPrefab);
		EventDispatcher::SpawnGameObject(pPrefab->Instantiate(), Vec3::Zero);
	}

	{
		// Camera	
		cGameObject* pObj = new cGameObject;
		cCom_Camera* Cam = pObj->AddComponent<cCom_Camera>();

		pObj->AddComponent(Cam);
		pObj->Camera()->SetCamIndex(eCAMERA_INDEX::MAIN);
		pObj->Camera()->SetProjType(ePROJ_TYPE::ORTHOGRAPHY);
		//pObj->AddComponent(new cTransform);

		pObj->AddComponent(cComMgr::GetInst()->GetNewCom(strKey_Com::cScript_MainCamSC_InGame));

		EventDispatcher::SpawnGameObject(pObj, Vec3(0.f, 0.f, 0.f), SC::LAYER_INFO::Camera);
	}
}

//void LoadRes(eRES_TYPE _eResType)
//{
//	cResMgr* pResMgr = cResMgr::GetInst();
//	//우선 테스트를 위해서 모든 리소스를 순회돌면서 로드해준다.
//	//텍스처 로드
//	std::filesystem::path ResPath = cPathMgr::GetInst()->GetPathRel_Resource(_eResType);
//
//	std::filesystem::recursive_directory_iterator RIter;
//	try
//	{
//		RIter = std::filesystem::recursive_directory_iterator(ResPath);
//
//		for (RIter; RIter != std::filesystem::end(RIter); ++RIter)
//		{
//			if (true == RIter->is_directory())
//				continue;
//			const auto& RelativePath = std::filesystem::relative(RIter->path(), ResPath);
//			pResMgr->Load(_eResType, RelativePath);
//			//pResMgr->Load<cTexture>(RelativePath);
//		}
//	}
//	catch (const std::filesystem::filesystem_error& error)
//	{
//		MessageBoxA(nullptr, error.what(), NULL, MB_OK);
//		throw(error);
//	}
//}

