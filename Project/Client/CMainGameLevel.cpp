#include "pch.h"
#include "CMainGameLevel.h"

#include <Engine/global.h>

#include <Engine/CDevice.h>

#include <Engine/CLevel.h>
#include <Engine/CLevelMgr.h>

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

#include <Script/strKey_Script.h>
#include <Script/strKey_Texture.h>
#include <Script/strKey_Shader.h>

#include <Script/CScript_FSM_Move_Ground.h>
#include <Script/CScript_MouseCursor.h>

#include <Engine/CScriptMgr.h>


#include <Engine/CTimeMgr.h>

#include <Engine/CResMgr.h>



#include <Engine/CRandMgr.h>

#include <Engine/EventDispatcher.h>

#include <Engine/jsoncpp.h>

#include "ManualEdit.h"
#include "strKey_Prefab.h"

namespace INGAME_LAYER_INFO
{
	enum idx
	{
		_00,
		TileMap,
		_03,
		GroundUnitShadow,
		_04,
		GroundUnitBack,
		_06,
		GroundUnitMain,
		_08,
		GroundUnitEffects,
		_10,
		GroundUnitAttack,
		_12,
		AirUnitShadow,
		_14,
		AirUnitBack,
		_16,
		AirUnitMain,
		_18,
		AirUnitBooster,
		_20,
		AirUnitAttack,
		_22,
		_23,
		_24,
		_25,
		_26,
		_27,
		_28,
		_29,
		MouseCursor,
		_31,
		END
	};

	constexpr inline const char* strLayerName[32] = {
	"", //00
	"TileMap",	//01
	"",	//02
	"GroundUnitShadow",	//03
	"",	//04
	"GroundUnitBack",	//05
	"",	//06
	"GroundUnitMain",	//07
	"",	//08
	"GroundUnitEffects",	//09
	"",	//10
	"GroundUnitAttack",	//11
	"",	//12
	"AirUnitShadow",	//13
	"",	//14
	"AirUnitBack",	//15
	"",	//16
	"AirUnitMain",	//17
	"",	//18
	"AirUnitBooster",	//19
	"",	//20
	"AirUnitAttack",	//21
	"",	//22
	"",	//23
	"",	//24
	"",	//25
	"",	//26
	"",	//27
	"",	//28
	"",	//29
	"MouseCursor",	//30
	"",	//31
	};
}



void CreateMainGame()
{
	LoadRes(eRES_TYPE::TEXTURE);
	LoadRes(eRES_TYPE::MATERIAL);

	ManualEdit::Edit();

	LoadRes(eRES_TYPE::PREFAB);

	for (int i = 0; i < 1; ++i)
	{
		Ptr<CPrefab> MarinePrefab = CResMgr::GetInst()->FindRes<CPrefab>(strKey_RES_PREFAB::MARINE);
		CGameObject* Marine = MarinePrefab->Instantiate();

		float randx = CRandMgr::GetInst()->GetRand<float>(-640.f, 640.f);
		float randy = CRandMgr::GetInst()->GetRand<float>(-320.f, 320.f);
		EventDispatcher::SpawnGameObject(Marine, Vec3(randx, randy, 0.f), 1);

		CScript_FSM_Move_Ground* pMoveGround = static_cast<CScript_FSM_Move_Ground*>(Marine->ScriptHolder()->FindScript(strKey_SCRIPTS::FSM_MOVE_GROUND));

		pMoveGround->SetSpeed(100.f);
	}


	CLevel* pLevel = CLevelMgr::GetInst()->GetCurLevel();
	CCollisionMgr::GetInst()->AddLayerInteraction2D(0, 1);

	//CCollisionMgr::GetInst()->AddLayerInteraction2D(INGAME_LAYER_INFO::GroundUnitMain, INGAME_LAYER_INFO::MouseCursor);
	CCollisionMgr::GetInst()->AddLayerInterAction2DAll(INGAME_LAYER_INFO::MouseCursor);

	CCollisionMgr::GetInst()->AddLayerInteraction2D(INGAME_LAYER_INFO::GroundUnitMain, INGAME_LAYER_INFO::GroundUnitMain);

	for (int i = 0; i < (int)INGAME_LAYER_INFO::idx::END; ++i)
	{
		pLevel->SetLayerName(i, string(INGAME_LAYER_INFO::strLayerName[i]));
	}

	Ptr<CMesh> CircleMesh = CResMgr::GetInst()->FindRes<CMesh>("CircleMesh");
	Ptr<CMesh> RectMesh = CResMgr::GetInst()->FindRes<CMesh>("RectMesh");
	Ptr<CTexture> Fighter = CResMgr::GetInst()->FindRes<CTexture>("Fighter");

	{
		// Camera
		CGameObject* pObj = new CGameObject;
		pObj->SetName("Camera");
		CCamera* Cam = new CCamera;
		pObj->AddComponent(Cam);
		pObj->Camera()->SetCamIndex(eCAMERA_INDEX::MAIN);
		pObj->Camera()->SetProjType(ePROJ_TYPE::ORTHOGRAPHY);
		//pObj->AddComponent(new CTransform);
		pObj->AddScript(CScriptMgr::GetInst()->GetNewScript(strKey_SCRIPTS::CAMERAMOVE));

		EventDispatcher::SpawnGameObject(pObj, Vec3(0.f, 0.f, -100.f), 1);
		//pLevel->AddGameObject(pObj, 1);
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

//void LoadAllTexture()
//{
//	CResMgr* pResMgr = CResMgr::GetInst();
//	//우선 테스트를 위해서 모든 리소스를 순회돌면서 로드해준다.
//	//텍스처 로드
//	std::filesystem::path ResPath = CPathMgr::GetInst()->GetPathRel_Resource(eRES_TYPE::TEXTURE);
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
//			pResMgr->Load<CTexture>(RelativePath);
//		}
//	}
//	catch (const std::filesystem::filesystem_error& error)
//	{
//		MessageBoxA(nullptr, error.what(), NULL, MB_OK);
//		throw(error);
//	}
//}
//
//void LoadUserMtrl()
//{
//	CResMgr* pResMgr = CResMgr::GetInst();
//	//우선 테스트를 위해서 모든 리소스를 순회돌면서 로드해준다.
//	//텍스처 로드
//	std::filesystem::path ResPath = CPathMgr::GetInst()->GetPathRel_Resource(eRES_TYPE::MATERIAL);
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
//			pResMgr->Load<CMaterial>(RelativePath);
//		}
//	}
//	catch (const std::filesystem::filesystem_error& error)
//	{
//		MessageBoxA(nullptr, error.what(), NULL, MB_OK);
//		throw(error);
//	}
//}

void LoadAnim()
{

}

bool CreateUserGraphicsShader()
{

	return true;
}

bool CreateUserComputeShader()
{
	return false;
}
