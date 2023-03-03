#include "pch.h"
#include "CTestLevel.h"

#include <Engine/CDevice.h>


#include <Engine/CLevel.h>
#include <Engine/CLevelMgr.h>

#include <Engine/ptr.h>
#include <Engine/CTexture.h>
#include <Engine/CResMgr.h>
#include <Engine/strKeys.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>
#include <Engine/CCollisionMgr.h>
#include <Engine/CTimeMgr.h>

#include <Engine/CComputeShader.h>
#include <Engine/CCS_SetColor.h>
#include <Engine/CCS_SCMapLoader.h>

#include <Script/CScript_Player.h>
#include <Script/CScript_MouseCursor.h>
#include <Script/CScript_Bullet.h>
#include <Script/CScript_Debug.h>

void CreateTestLevel()
{
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurLevel();

	pLevel->SetLayerName(0, "DefaultLayer");
	pLevel->SetLayerName(1, "Layer 1");


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

	for (int i = 0; i < 5; ++i)
	{
		CGameObject* pPlayer = new CGameObject;
		pPlayer->SetName("Player");
		pPlayer->AddComponent(new CTransform);
		pPlayer->Transform()->SetSize(Vec3(84.f, 84.f, 1.f));
		pPlayer->Transform()->SetLockRotation(true);
		pPlayer->AddComponent(new CMeshRender);

		if (0 == i)
			pPlayer->AddScript(new CScript_Player);

		Ptr<CMaterial> PlayerMtrl = CResMgr::GetInst()->FindRes<CMaterial>(RESOURCE::MATERIAL::MARINE);

		Vec4 ColorKey(0.f, 0.f, 0.f, 0.f);
		pPlayer->SetMtrlScalarParam(MTRL_SCALAR_STD2D_COLORKEY, ColorKey);
		pPlayer->SetMtrlScalarParam_IntFlag(MTRL_SCALAR_STD2D_FLAG, (INT32)eMTRL_SCALAR_STD2D_FLAG::USE_COLOR_KEY, true);

		pPlayer->MeshRender()->SetMesh(RectMesh);
		pPlayer->MeshRender()->SetMaterial(PlayerMtrl);

		//pPlayer->AddComponent(new CLight2D);
		//pPlayer->Light2D()->SetLightType(eLIGHT_TYPE::POINT);

		pPlayer->AddComponent(new CAnimator2D);

		Ptr<CAnim2DAtlas> pAnimAtlas = CResMgr::GetInst()->FindRes<CAnim2DAtlas>(RESOURCE::TEXTURE::MARINE_ATLAS);
		pPlayer->Animator2D()->AddAtlasTex(eMTRLDATA_PARAM_TEX::_0, pAnimAtlas);
		pPlayer->Animator2D()->Play(RESOURCE::ANIM2D::MARINE_ATTACK, eANIM_LOOPMODE::NORMAL_LOOP, false);

		if (0 == i)
			pPlayer->AddComponent(new CCollider2D_Point);
		else
			pPlayer->AddComponent(new CCollider2D_OBB);

		::SpawnGameObject(pPlayer, Vec3(-600.f + 1200.f * CTimeMgr::GetInst()->GetRandomNorm(), -300.f + 600.f * CTimeMgr::GetInst()->GetRandomNorm(), 1.f), 1);
	}



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
	//	pPlayer->AddScript(new CScript_Player);

	//	Ptr<CMaterial> PlayerMtrl = CResMgr::GetInst()->FindRes<CMaterial>(RESOURCE::MATERIAL::STD2D_LIGHT);

	//	Vec4 ColorKey(0.f, 0.f, 0.f, 0.f);
	//	pPlayer->SetMtrlScalarParam(MTRL_SCALAR_STD2D_COLORKEY, ColorKey);

	//	pPlayer->MeshRender()->SetMesh(RectMesh);
	//	pPlayer->MeshRender()->SetMaterial(PlayerMtrl);

	//	pPlayer->AddComponent(new CAnimator2D);

	//	Ptr<CAnim2DAtlas> pAnimAtlas = CResMgr::GetInst()->FindRes<CAnim2DAtlas>(RESOURCE::TEXTURE::CORSAIR_ATLAS);
	//	pPlayer->Animator2D()->AddAtlasTex(eMTRLDATA_PARAM_TEX::_0, pAnimAtlas);
	//	pPlayer->Animator2D()->Play(RESOURCE::ANIM2D::CORSAIRMOVE, eANIM_LOOPMODE::NORMAL_LOOP, false);


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
	//	pTestObj1->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(RESOURCE::MESH::RECT));
	//	Ptr<CMaterial> TestMtrl = CResMgr::GetInst()->FindRes<CMaterial>(RESOURCE::MATERIAL::TEST);
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
	//	pParticle->SetParticleCS(RESOURCE::SHADER::COMPUTE::PARTICLE_UPDATE_RAINDROP);

	//	Ptr<CTexture> pHOSTEX = CResMgr::GetInst()->FindRes<CTexture>(RESOURCE::TEXTURE::HOS);
	//	pParticle->GetCurMaterial()->SetTexParam(eMTRLDATA_PARAM_TEX::_0, pHOSTEX);
	//	pParticle->CreateParticle();
	//	pParticleObj->AddComponent(pParticle);

	//	pParticleObj->AddScript(new CScript_Player);

	//	SpawnGameObject(pParticleObj, Vec3(-100.f, -100.f, 10.f), 1);
	//}



	//CGameObject* pTestObj4 = new CGameObject;
	//pTestObj4->SetName("Test");
	//pTestObj4->AddComponent(new CTransform);

	//pTestObj3->AddChildObj(pTestObj4);
	//SpawnGameObject(pTestObj4, Vec3(-100.f, -100.f, 10.f), 1);


	//{//Tilemap

	//	CGameObject* pTilemap = new CGameObject;
	//	pTilemap->SetName(RESOURCE::TEXTURE::TILE_ATLAS);

	//	pTilemap->AddComponent(new CTransform);
	//	pTilemap->AddComponent(new CTilemap);

	//	pTilemap->Transform()->SetSize(Vec3(500.f, 500.f, 1.f));

	//	pTilemap->Tilemap()->GetCurMaterial()->SetTexParam(eMTRLDATA_PARAM_TEX::_0, CResMgr::GetInst()->FindRes<CTexture>(RESOURCE::TEXTURE::TILE_ATLAS));
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
		pObj->AddComponent(new CTransform);
		//pObj->AddScript(new CScript_CameraMove);

		SpawnGameObject(pObj, Vec3(0.f, 0.f, -100.f), 1);
		//pLevel->AddGameObject(pObj, 1);

		
	}

	{//Prefab
		CGameObject* pPrefab = new CGameObject;

		pPrefab->AddComponent(new CMeshRender);
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>("TestMtrl");
		Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>("HOS");

		pMtrl->SetTexParam(eMTRLDATA_PARAM_TEX::_0, pTex);
		Ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>("CircleMesh");
		pPrefab->MeshRender()->SetMaterial(pMtrl);
		pPrefab->MeshRender()->SetMesh(pMesh);

		pPrefab->AddScript(new CScript_Bullet);

		pPrefab->AddComponent(new CTransform);

		Ptr<CPrefab> pResPrefab = new CPrefab;
		pResPrefab->RegisterPrefab(pPrefab);

		CResMgr::GetInst()->AddRes("Bullet", pResPrefab);
	}


	Ptr<CCS_SCMapLoader> pMapLoader = CResMgr::GetInst()->FindRes<CComputeShader>(RESOURCE::SHADER::COMPUTE::SC_MAP_LOADER);

	pMapLoader->Execute();

	

	Ptr<CTexture> MapTex = pMapLoader->GetMap();

	CGameObject* MapObj = new CGameObject;
	MapObj->AddComponent(new CTransform);
	MapObj->Transform()->SetSize(Vec3(MapTex->GetSize(), 1.f));
	CMeshRender* pMesh = new CMeshRender;
	MapObj->AddComponent(pMesh);
	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(RESOURCE::MATERIAL::STD2D);
	pMtrl->SetTexParam(eMTRLDATA_PARAM_TEX::_0, MapTex);
	pMesh->SetMaterial(pMtrl);

	pMesh->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(RESOURCE::MESH::RECT));

	::SpawnGameObject(MapObj, Vec3(0.f, 0.f, 1000.f), 0);
}
