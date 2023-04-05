#include "pch.h"
#include "CMainGameLevel.h"

#include <Engine/global.h>

#include <Engine/CDevice.h>

#include <Engine/CLevel.h>
#include <Engine/CLevelMgr.h>

#include <Engine/ptr.h>
#include <Engine/CTexture.h>
#include <Engine/CResMgr.h>
#include <Engine/strKeyDefault.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>
#include <Engine/CCollisionMgr.h>
#include <Engine/CTimeMgr.h>

#include <Engine/CComputeShader.h>
#include <Engine/CCS_SetColor.h>
#include <Engine/CCS_SCMapLoader.h>

#include <Script/strKeyScript.h>
#include <Script/strKeyTexture.h>
#include <Script/strKeyShader.h>

#include <Engine/CScriptMgr.h>
#include <Script/CScript_MouseCursor.h>

#include <Engine/CTimeMgr.h>

#include <Engine/CResMgr.h>



#include <Engine/CRandMgr.h>

#include <Engine/EventDispatcher.h>

#include <Engine/jsoncpp.h>

#include "EditAnim.h"

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

	constexpr std::string_view strLayerName[32] = {
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

	CLevel* pLevel = CLevelMgr::GetInst()->GetCurLevel();

	//CCollisionMgr::GetInst()->AddLayerInteraction2D(INGAME_LAYER_INFO::GroundUnitMain, INGAME_LAYER_INFO::MouseCursor);
	CCollisionMgr::GetInst()->AddLayerInterAction2DAll(INGAME_LAYER_INFO::MouseCursor);

	CCollisionMgr::GetInst()->AddLayerInteraction2D(INGAME_LAYER_INFO::GroundUnitMain, INGAME_LAYER_INFO::GroundUnitMain);

	for (int i = 0; i < (int)INGAME_LAYER_INFO::idx::END; ++i)
	{
		pLevel->SetLayerName(i, string(INGAME_LAYER_INFO::strLayerName[i]));
	}

	LoadAllTexture();
	LoadUserMtrl();
	AnimEditor::EditAnim();

	CResMgr* pResMgr = CResMgr::GetInst();

	Ptr<CMaterial> SCUnitMtrl = new CMaterial;

	Ptr<CTexture> pTexMarine = pResMgr->FindRes<CTexture>(string(strKey_Texture::TERRAN::MARINE_BMP));

	Ptr<CGraphicsShader> pSCUnitShader = pResMgr->FindRes<CGraphicsShader>(string(RES_SHADER::GRAPHICS::SCUNITGROUND));
	SCUnitMtrl->SetShader(pSCUnitShader);
	SCUnitMtrl->SetTexParam(eMTRLDATA_PARAM_TEX::_0, pTexMarine);
	SCUnitMtrl->SetKey("SCUnitMtrl.json");

	Json::Value SaveFile;
	SCUnitMtrl->SaveJson(&SaveFile);

	for(int i = 0; i < 1; ++i)
	{
		CGameObject* TestObj = new CGameObject;
		TestObj->SetName("TestObj");
		
		TestObj->AddComponent(new CMeshRender);
		TestObj->MeshRender()->SetMaterial(SCUnitMtrl);
		TestObj->MeshRender()->SetMesh(pResMgr->FindRes<CMesh>(string(RES_DEFAULT::MESH::RECT)));

		TestObj->AddComponent(new CAnimator2D);
		
		Ptr<CAnim2DAtlas> animAtlas = pResMgr->FindRes<CAnim2DAtlas>(string(strKey_Texture::TERRAN::MARINE_BMP));
		
		TestObj->Animator2D()->AddAtlasTex(eMTRLDATA_PARAM_TEX::_0, animAtlas);

		//TestObj->Animator2D()->Play("Marine_Move", eANIM_LOOPMODE::NORMAL_LOOP, false);
		
		TestObj->Transform()->SetSize(Vec3(TestObj->Animator2D()->GetCurFrameSize(), 1.f));

		if (i != 0)
		{
			CCollider2D_Rect* pCol = new CCollider2D_Rect;
			Vec2 xy = Vec2(32.f, 32.f) * Vec2(2.f, 1.5f);
			pCol->SetSCBuildingSize(2u, 2u, Vec4(-7, 0, 8, 7));
			TestObj->AddComponent(pCol);


			float x = CRandMgr::GetInst()->GetRand(0.f, 1.f) * 1280.f;
			float y = CRandMgr::GetInst()->GetRand(0.f, 1.f) * 640.f;
			EventDispatcher::SpawnGameObject(TestObj, Vec3(-640.f + x, -320.f + y, 0.f), INGAME_LAYER_INFO::GroundUnitMain);
		}
		else
		{	
			CCollider2D_Circle* pCircle = new CCollider2D_Circle;
			pCircle->SetRadius(50.f);
			//CCollider2D_Rect* pCol = new CCollider2D_Rect;
			//Vec2 xy = Vec2(32.f, 32.f) * Vec2(2.f, 1.5f);
			//pCol->SetSCBuildingSize(2u, 2u, Vec4(-7, 0, 8, 7));
			TestObj->AddComponent(pCircle);

			EventDispatcher::SpawnGameObject(TestObj, Vec3(0.f, 0.f, 0.f), INGAME_LAYER_INFO::GroundUnitMain);
		}
		TestObj->AddScript(CScriptMgr::GetInst()->GetNewScript(string(SCRIPTS::MARINE)));

		Vec4 ColorKey(0.f, 0.f, 0.f, 0.f);
		TestObj->SetMtrlScalarParam(MTRL_SCALAR_STD2D_COLORKEY, &ColorKey);
		TestObj->SetMtrlScalarParam_IntFlag(MTRL_SCALAR_STD2D_FLAG, (INT32)eMTRL_SCALAR_STD2D_FLAG::USE_COLOR_KEY, true);

		//std::filesystem::path TestSave = "TestSave.json";
		//CPrefab* pPrefab = new CPrefab;

		//pPrefab->RegisterPrefab(TestObj);

		//pPrefab->SetKey(TestSave.string());

		//pPrefab->Save(TestSave);

		//delete pPrefab;
	}

	{
		//std::filesystem::path TestSave = "TestSave.json";
		//Ptr<CPrefab> pPrefab = new CPrefab;
		//pPrefab->Load(TestSave);

		//CResMgr::GetInst()->AddRes<CPrefab>(pPrefab->GetKey(), pPrefab);

		//CGameObject* newobj = pPrefab->Instantiate();
		//
		//EventDispatcher::SpawnGameObject(newobj, Vec3(10.f, 10.f, 0.f), 0);
	}
	




	//Ptr<CTexture> pCreateTex = CResMgr::GetInst()->CreateTexture(
	//	"SampleTexture"
	//	, 1280, 768
	//	, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM
	//	, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS
	//	, D3D11_USAGE_DEFAULT);

	//Ptr<CCS_SetColor> pCS = CResMgr::GetInst()->FindRes<CComputeShader>("CSSetColor");
	//pCS->SetTargetTexture(pCreateTex);
	//pCS->SetColor(Vec3(1.f, 0.f, 0.f));
	//pCS->Execute();



	Ptr<CMesh> CircleMesh = CResMgr::GetInst()->FindRes<CMesh>("CircleMesh");
	Ptr<CMesh> RectMesh = CResMgr::GetInst()->FindRes<CMesh>("RectMesh");
	Ptr<CTexture> Fighter = CResMgr::GetInst()->FindRes<CTexture>("Fighter");
	// 오브젝트 생성

	//for (int i = 0; i < 5; ++i)
	//{
	//	CGameObject* pPlayer = new CGameObject;
	//	pPlayer->SetName("Player");
	//	pPlayer->AddComponent(new CTransform);
	//	pPlayer->Transform()->SetSize(Vec3(84.f, 84.f, 1.f));
	//	pPlayer->Transform()->SetLockRotation(true);
	//	pPlayer->AddComponent(new CMeshRender);

	//	if (0 == i)
	//		pPlayer->AddScript(CScriptMgr::GetInst()->GetNewScript(SCRIPTS::PLAYER));
	//	
	//	Ptr<CMaterial> PlayerMtrl = CResMgr::GetInst()->FindRes<CMaterial>(RES_DEFAULT::MATERIAL::MARINE);

	//	Vec4 ColorKey(0.f, 0.f, 0.f, 0.f);
	//	pPlayer->SetMtrlScalarParam(MTRL_SCALAR_STD2D_COLORKEY, ColorKey);
	//	pPlayer->SetMtrlScalarParam_IntFlag(MTRL_SCALAR_STD2D_FLAG, (INT32)eMTRL_SCALAR_STD2D_FLAG::USE_COLOR_KEY, true);

	//	pPlayer->MeshRender()->SetMesh(RectMesh);
	//	pPlayer->MeshRender()->SetMaterial(PlayerMtrl);

	//	//pPlayer->AddComponent(new CLight2D);
	//	//pPlayer->Light2D()->SetLightType(eLIGHT_TYPE::POINT);

	//	pPlayer->AddComponent(new CAnimator2D);

	//	Ptr<CAnim2DAtlas> pAnimAtlas = CResMgr::GetInst()->FindRes<CAnim2DAtlas>(RES_DEFAULT::TEXTURE::MARINE_ATLAS);
	//	pPlayer->Animator2D()->AddAtlasTex(eMTRLDATA_PARAM_TEX::_0, pAnimAtlas);
	//	pPlayer->Animator2D()->Play(RES_DEFAULT::ANIM2D::MARINE_ATTACK, eANIM_LOOPMODE::NORMAL_LOOP, false);

	//	if (0 == i)
	//		pPlayer->AddComponent(new CCollider2D_Point);
	//	else
	//		pPlayer->AddComponent(new CCollider2D_OBB);

	//	::SpawnGameObject(pPlayer, Vec3(-600.f + 1200.f * CTimeMgr::GetInst()->GetRandomNorm(), -300.f + 600.f * CTimeMgr::GetInst()->GetRandomNorm(), 1.f), 1);
	//}



	CCollisionMgr::GetInst()->AddLayerInteraction2D(0, 1);

	//for (int i = 0; i < 1000; ++i)
	//{
	//	
	//	CGameObject* pPlayer = new CGameObject;
	//	pPlayer->SetName("Corsair");
	//	pPlayer->AddComponent(new CTransform);
	//	pPlayer->Transform()->SetSize(Vec3(100.f, 100.f, 1.f));
	//	pPlayer->Transform()->SetLockRotation(true);
	//	pPlayer->AddComponent(new CMeshRender);

	//	//if(1 == i)
	//	pPlayer->AddScript(new CScript_SCGroundUnitBase);

	//	Ptr<CMaterial> PlayerMtrl = CResMgr::GetInst()->FindRes<CMaterial>(RES_DEFAULT::MATERIAL::STD2D_LIGHT);

	//	Vec4 ColorKey(0.f, 0.f, 0.f, 0.f);
	//	pPlayer->SetMtrlScalarParam(MTRL_SCALAR_STD2D_COLORKEY, ColorKey);

	//	pPlayer->MeshRender()->SetMesh(RectMesh);
	//	pPlayer->MeshRender()->SetMaterial(PlayerMtrl);

	//	pPlayer->AddComponent(new CAnimator2D);

	//	Ptr<CAnim2DAtlas> pAnimAtlas = CResMgr::GetInst()->FindRes<CAnim2DAtlas>(RES_DEFAULT::TEXTURE::CORSAIR_ATLAS);
	//	pPlayer->Animator2D()->AddAtlasTex(eMTRLDATA_PARAM_TEX::_0, pAnimAtlas);
	//	pPlayer->Animator2D()->Play(RES_DEFAULT::ANIM2D::CORSAIRMOVE, eANIM_LOOPMODE::NORMAL_LOOP, false);


	//	::SpawnGameObject(pPlayer, Vec3(-600.f + 1200.f * CTimeMgr::GetInst()->GetRandomNorm(), -300.f + 600.f * CTimeMgr::GetInst()->GetRandomNorm(), 1.f), 1);
	//}


	//for(int j = 0; j < 10; ++j)
	//{
	//	CGameObject* LightObj = new CGameObject;

	//	LightObj->AddComponent(new CTransform);
	//	LightObj->AddComponent(new CLight2D);

	//	LightObj->Light2D()->SetLightType(eLIGHT_TYPE::POINT);
	//	LightObj->Light2D()->SetLightRadius(200.f);


	//	::SpawnGameObject(LightObj, Vec3(-600.f + 1200.f * CTimeMgr::GetInst()->GetRandomNorm() , -300.f + 600.f * CTimeMgr::GetInst()->GetRandomNorm(), 1.f), 1);
	//}




	//{
	//	//Test Object 1
	//	CGameObject * pTestObj1 = new CGameObject;
	//	pTestObj1->SetName("Test Object");
	//	pTestObj1->AddComponent(new CTransform);
	//	pTestObj1->Transform()->SetSize(Vec3(100.f, 100.f, 1.f));
	//	pTestObj1->AddComponent(new CMeshRender);
	//	pTestObj1->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(RES_DEFAULT::MESH::RECT));
	//	Ptr<CMaterial> TestMtrl = CResMgr::GetInst()->FindRes<CMaterial>(RES_DEFAULT::MATERIAL::TEST);
	//	TestMtrl->SetTexParam(eMTRLDATA_PARAM_TEX::_0, Fighter);
	//	//pTestObj1->SetMtrlScalarParam((eMTRLDATA_PARAM_SCALAR)MTRL_SCALAR_STD2D_COLORKEY, ColorKey);
	//	pTestObj1->MeshRender()->SetMaterial(TestMtrl);

	//	::SpawnGameObject(pTestObj1, Vec3(100.f, 0.f, 10.f), 1);
	//	::AddChildObj(pPlayer, pTestObj1);
	//}

	

	// Test Object 2
	//CGameObject* pTestObj2 = new CGameObject;
	//pTestObj2->SetName(L"Test Object");
	//pTestObj2->AddComponent(new CTransform);
	////pTestObj2->Transform()->SetRelativePos();
	//pTestObj2->Transform()->SetRotInheritance(false);
	//pTestObj2->Transform()->SetScaleInheritance(false);
	//pTestObj2->Transform()->SetSize(Vec3(2048.f, 4096.f, 1.f));
	//pTestObj2->AddComponent(new CMeshRender);
	//pTestObj2->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>("RectMesh"));

	//Ptr<CMaterial> pLightMtrl = CResMgr::GetInst()->FindRes<CMaterial>("std2DLightMtrl");

	//pLightMtrl->SetTexParam(eTEX_0, CResMgr::GetInst()->FindRes<CTexture>("beheaded"));
	//pLightMtrl->SetMtrlScalarParam(MTRL_SCALAR_STD2D_COLORKEY, Vec4(0.f, 0.f, 0.f, 1.f));
	//pLightMtrl->SetTexParam(eTEX_1, CResMgr::GetInst()->FindRes<CTexture>("beheaded_n"));

	//pTestObj2->MeshRender()->SetMaterial(pLightMtrl);

	//pTestObj2->AddScript(new CScript_TestObj);
	//
	//::SpawnGameObject(pTestObj2, Vec3(300.f, 300.f, 10.f), 1);



	//::AddChildObj(pTestObj1, pTestObj2);
	//pTestObj1->AddChildObj(pTestObj2);
	//SpawnGameObject(pTestObj2, Vec3(0.f, 100.f, 10.f), )
	//pLevel->AddGameObject(pTestObj2, 1);

	//int square = 10;
	//static float startxy = -(110 * square) * 0.5f;
	//for(int i = 0; i < square; ++i)
	//{
	//	for (int j = 0; j < square; ++j)
	//	{
	//		// Test Object 3
	//		CGameObject* pTestObj3 = new CGameObject;

	//		pTestObj3->SetName(L"Test Object");
	//		pTestObj3->AddComponent(new CTransform);
	//		//pTestObj3->Transform()->SetRelativePos(-5000.f + 110.f * i, -300.f + 110.f * j, 10.f);
	//		pTestObj3->Transform()->SetSize(Vec3(100.f, 100.f, 1.f));
	//		pTestObj3->AddComponent(new CMeshRender);
	//		pTestObj3->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//		Ptr<CMaterial> TestMtrl3 = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
	//		TestMtrl->SetMtrlScalarParam((eMTRLDATA_PARAM_SCALAR)MTRL_SCALAR_STD2D_COLORKEY, ColorKey);
	//		pTestObj3->MeshRender()->SetMaterial(TestMtrl);
	//		pTestObj3->AddComponent(new CCollider2D_OBB);

	//		//pTestObj3->AddScript(new CScript_TestObj);

	//		SpawnGameObject(pTestObj3, Vec3(startxy + 110.f * i, startxy + 110.f * j, 10.f), 1);
	//		//pLevel->AddGameObject(pTestObj3, 1);
	//	}

	//}


	// Test Object 3
	//CGameObject* pTestObj3 = new CGameObject;
	//pTestObj3->SetName("Test Object");
	//pTestObj3->AddComponent(new CTransform);
	//pTestObj3->Transform()->SetSize(Vec3(100.f, 100.f, 1.f));

	//pTestObj3->AddComponent(new CMeshRender);
	//pTestObj3->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>("RectMesh"));
	//Ptr<CMaterial> TestMtrl3 = CResMgr::GetInst()->FindRes<CMaterial>("TestMtrl");
	//TestMtrl3->SetTexParam(eMTRLDATA_PARAM_TEX::_0, pCreateTex);
	//pTestObj3->MeshRender()->SetMaterial(TestMtrl3);

	////pTestObj3->AddComponent(new CParticleSystem);


	//pTestObj3->AddComponent(new CCollider2D_OBB);

	////pTestObj3->AddScript(new CScript_TestObj);

	//SpawnGameObject(pTestObj3, Vec3(100.f, 100.f, 10.f), 1);
	////pLevel->AddGameObject(pTestObj3, 1);




	//{
	//	//Particle
	//	CGameObject* pParticleObj = new CGameObject;
	//	pParticleObj->SetName("ParticleObj");
	//	pParticleObj->AddComponent(new CTransform);
	//	pParticleObj->Transform()->SetSize(Vec3(100.f, 100.f, 1.f));

	//	CParticleSystem* pParticle = new CParticleSystem;
	//	pParticle->SetParticleCS(RES_DEFAULT::SHADER::COMPUTE::PARTICLE_UPDATE_RAINDROP);

	//	Ptr<CTexture> pHOSTEX = CResMgr::GetInst()->FindRes<CTexture>(RES_DEFAULT::TEXTURE::HOS);
	//	pParticle->GetCurMaterial()->SetTexParam(eMTRLDATA_PARAM_TEX::_0, pHOSTEX);
	//	pParticle->CreateParticle();
	//	pParticleObj->AddComponent(pParticle);

	//	pParticleObj->AddScript(new CScript_SCGroundUnitBase);

	//	SpawnGameObject(pParticleObj, Vec3(-100.f, -100.f, 10.f), 1);
	//}



	//CGameObject* pTestObj4 = new CGameObject;
	//pTestObj4->SetName("Test");
	//pTestObj4->AddComponent(new CTransform);

	//pTestObj3->AddChildObj(pTestObj4);
	//SpawnGameObject(pTestObj4, Vec3(-100.f, -100.f, 10.f), 1);


	//{//Tilemap

	//	CGameObject* pTilemap = new CGameObject;
	//	pTilemap->SetName(RES_DEFAULT::TEXTURE::TILE_ATLAS);

	//	pTilemap->AddComponent(new CTransform);
	//	pTilemap->AddComponent(new CTilemap);

	//	pTilemap->Transform()->SetSize(Vec3(500.f, 500.f, 1.f));

	//	pTilemap->Tilemap()->GetCurMaterial()->SetTexParam(eMTRLDATA_PARAM_TEX::_0, CResMgr::GetInst()->FindRes<CTexture>(RES_DEFAULT::TEXTURE::TILE_ATLAS));
	//	pTilemap->Tilemap()->SetSliceSize(Vec2(0.125f, 0.166f));
	//	pTilemap->Tilemap()->SetTileCount(8, 8);

	//	::SpawnGameObject(pTilemap, Vec3(0.f, 0.f, 1000.f), 1);
	//}

	{
		// Camera
		CGameObject* pObj = new CGameObject;
		pObj->SetName("Camera");
		CCamera* Cam = new CCamera;
		pObj->AddComponent(Cam);
		pObj->Camera()->SetCamIndex(eCAMERA_INDEX::MAIN);
		pObj->Camera()->SetProjType(ePROJ_TYPE::ORTHOGRAPHY);
		//pObj->AddComponent(new CTransform);
		pObj->AddScript(CScriptMgr::GetInst()->GetNewScript(string(SCRIPTS::CAMERAMOVE)));

		EventDispatcher::SpawnGameObject(pObj, Vec3(0.f, 0.f, -100.f), 1);
		//pLevel->AddGameObject(pObj, 1);
	}

	{//Prefab
		//CGameObject* pPrefab = new CGameObject;

		//pPrefab->AddComponent(new CMeshRender);
		//Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>("TestMtrl");
		//Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>("HOS");

		//pMtrl->SetTexParam(eMTRLDATA_PARAM_TEX::_0, pTex);
		//Ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>("CircleMesh");
		//pPrefab->MeshRender()->SetMaterial(pMtrl);
		//pPrefab->MeshRender()->SetMesh(pMesh);

		//pPrefab->AddScript(new CScript_Bullet);

		//pPrefab->AddComponent(new CTransform);

		//Ptr<CPrefab> pResPrefab = new CPrefab;
		//pResPrefab->RegisterPrefab(pPrefab);

		//CResMgr::GetInst()->AddRes("Bullet", pResPrefab);
	}
}

void LoadAllTexture()
{
	CResMgr* pResMgr = CResMgr::GetInst();
	//우선 테스트를 위해서 모든 리소스를 순회돌면서 로드해준다.
	//텍스처 로드
	std::filesystem::path ResPath = CPathMgr::GetInst()->GetPathRel_Resource(eRES_TYPE::TEXTURE);

	std::filesystem::recursive_directory_iterator RIter;
	try
	{
		RIter = std::filesystem::recursive_directory_iterator(ResPath);

		for (RIter; RIter != std::filesystem::end(RIter); ++RIter)
		{
			if (true == RIter->is_directory())
				continue;
			const auto& RelativePath = std::filesystem::relative(RIter->path(), ResPath);
			pResMgr->Load<CTexture>(RelativePath);
		}
	}
	catch (const std::filesystem::filesystem_error& error)
	{
		MessageBoxA(nullptr, error.what(), NULL, MB_OK);
		throw(error);
	}
}

void LoadUserMtrl()
{
	CResMgr* pResMgr = CResMgr::GetInst();
	//우선 테스트를 위해서 모든 리소스를 순회돌면서 로드해준다.
	//텍스처 로드
	std::filesystem::path ResPath = CPathMgr::GetInst()->GetPathRel_Resource(eRES_TYPE::MATERIAL);

	std::filesystem::recursive_directory_iterator RIter;
	try
	{
		RIter = std::filesystem::recursive_directory_iterator(ResPath);

		for (RIter; RIter != std::filesystem::end(RIter); ++RIter)
		{
			if (true == RIter->is_directory())
				continue;
			const auto& RelativePath = std::filesystem::relative(RIter->path(), ResPath);
			pResMgr->Load<CMaterial>(RelativePath);
		}
	}
	catch (const std::filesystem::filesystem_error& error)
	{
		MessageBoxA(nullptr, error.what(), NULL, MB_OK);
		throw(error);
	}
}

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
