#include "pch.h"
#include "CLevelMgr.h"

#include "CLevel.h"
#include "CLayer.h"

#include "CGameObject.h"
#include "components.h"
#include "CMeshRender.h"

//Scripts
#include "CPlayerScript.h"
#include "CTestObjScript.h"
#include "CBulletScript.h"

#include "CResMgr.h"

#include "CCameraMoveScript.h"

//Camera Register
#include "CRenderMgr.h"

//충돌 레이어 등록 및 체크용
#include "CCollisionMgr.h"

//타일맵
#include "CTilemap.h"

//컴퓨트쉐이더 테스트
#include "CCS_SetColor.h"

#include "CParticleSystem.h"

#include "strKeys.h"

#include "CTimeMgr.h"

CLevelMgr::CLevelMgr()
	: m_pCurLevel(nullptr)
{
}

CLevelMgr::~CLevelMgr()
{
	if (nullptr != m_pCurLevel)
		delete m_pCurLevel;
}


CGameObject* CLevelMgr::FindObjectByName(const string& _Name)
{
	return m_pCurLevel->FindObjectByName(_Name);
}

void CLevelMgr::FindObjectALLByName(const string& _Name, vector<CGameObject*>& _vecObj)
{
	m_pCurLevel->FindObjectALLByName(_Name, _vecObj);
}

void CLevelMgr::init()
{
	m_pCurLevel = new CLevel;

	m_pCurLevel->SetLayerName(0, "DefaultLayer");
	m_pCurLevel->SetLayerName(1, "Layer 1");


	Ptr<CTexture> pCreateTex = CResMgr::GetInst()->CreateTexture(
		"SampleTexture"
		, 1280, 768
		, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM
		, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS
		, D3D11_USAGE_DEFAULT);

	Ptr<CCS_SetColor> pCS = CResMgr::GetInst()->FindRes<CComputeShader>("CSSetColor");
	pCS->SetTargetTexture(pCreateTex);
	pCS->SetColor(Vec3(1.f, 0.f, 0.f));
	pCS->Execute();

	

	Ptr<CMesh> CircleMesh = CResMgr::GetInst()->FindRes<CMesh>("CircleMesh");
	Ptr<CMesh> RectMesh = CResMgr::GetInst()->FindRes<CMesh>("RectMesh");
	Ptr<CTexture> Fighter = CResMgr::GetInst()->FindRes<CTexture>("Fighter");
	
	
	
	
	// 오브젝트 생성

	//for (int i = 0; i < 1000; ++i)
	int i = 1;
	{
		CGameObject* pPlayer = new CGameObject;
		pPlayer->SetName("Player");
		pPlayer->AddComponent(new CTransform);
		pPlayer->Transform()->SetSize(Vec3(84.f, 84.f, 1.f));
		pPlayer->Transform()->SetLockRotation(true);
		pPlayer->AddComponent(new CMeshRender);

		//if(1 == i)
			pPlayer->AddScript(new CPlayerScript);

		Ptr<CMaterial> PlayerMtrl = CResMgr::GetInst()->FindRes<CMaterial>(RESOURCE::MATERIAL::MARINE);

		Vec4 ColorKey(0.f, 0.f, 0.f, 0.f);
		pPlayer->SetScalarParam((eMTRLDATA_PARAM_SCALAR)MTRL_SCALAR_STD2D_COLORKEY, ColorKey);

		pPlayer->MeshRender()->SetMesh(RectMesh);
		pPlayer->MeshRender()->SetMaterial(PlayerMtrl);

		//pPlayer->AddComponent(new CLight2D);
		//pPlayer->Light2D()->SetLightType(eLIGHT_TYPE::POINT);

		pPlayer->AddComponent(new CAnimator2D);

		Ptr<CAnim2DAtlas> pAnimAtlas = CResMgr::GetInst()->FindRes<CAnim2DAtlas>(RESOURCE::TEXTURE::MARINE_ATLAS);
		pPlayer->Animator2D()->AddAtlasTex(eMTRLDATA_PARAM_TEX::_0, pAnimAtlas);
		pPlayer->Animator2D()->Play(RESOURCE::ANIM2D::MARINE_IDLE, eANIM_LOOPMODE::NONE, false);

		
		//pPlayer->AddComponent(new CCollider2D_OBB);

		::SpawnGameObject(pPlayer, Vec3(-600.f + 1200.f * CTimeMgr::GetInst()->GetRandomNorm(), -300.f + 600.f * CTimeMgr::GetInst()->GetRandomNorm(), 1.f), 1);
	}

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
	//	pPlayer->AddScript(new CPlayerScript);

	//	Ptr<CMaterial> PlayerMtrl = CResMgr::GetInst()->FindRes<CMaterial>(RESOURCE::MATERIAL::STD2D_LIGHT);

	//	Vec4 ColorKey(0.f, 0.f, 0.f, 0.f);
	//	pPlayer->SetScalarParam(MTRL_SCALAR_STD2D_COLORKEY, ColorKey);

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
	//	//pTestObj1->SetScalarParam((eMTRLDATA_PARAM_SCALAR)MTRL_SCALAR_STD2D_COLORKEY, ColorKey);
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
	//pLightMtrl->SetScalarParam(MTRL_SCALAR_STD2D_COLORKEY, Vec4(0.f, 0.f, 0.f, 1.f));
	//pLightMtrl->SetTexParam(eTEX_1, CResMgr::GetInst()->FindRes<CTexture>("beheaded_n"));

	//pTestObj2->MeshRender()->SetMaterial(pLightMtrl);

	//pTestObj2->AddScript(new CTestObjScript);
	//
	//::SpawnGameObject(pTestObj2, Vec3(300.f, 300.f, 10.f), 1);



	//::AddChildObj(pTestObj1, pTestObj2);
	//pTestObj1->AddChildObj(pTestObj2);
	//SpawnGameObject(pTestObj2, Vec3(0.f, 100.f, 10.f), )
	//m_pCurLevel->AddGameObject(pTestObj2, 1);

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
	//		TestMtrl->SetScalarParam((eMTRLDATA_PARAM_SCALAR)MTRL_SCALAR_STD2D_COLORKEY, ColorKey);
	//		pTestObj3->MeshRender()->SetMaterial(TestMtrl);
	//		pTestObj3->AddComponent(new CCollider2D_OBB);

	//		//pTestObj3->AddScript(new CTestObjScript);

	//		SpawnGameObject(pTestObj3, Vec3(startxy + 110.f * i, startxy + 110.f * j, 10.f), 1);
	//		//m_pCurLevel->AddGameObject(pTestObj3, 1);
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

	////pTestObj3->AddScript(new CTestObjScript);

	//SpawnGameObject(pTestObj3, Vec3(100.f, 100.f, 10.f), 1);
	////m_pCurLevel->AddGameObject(pTestObj3, 1);



		
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

	//	pParticleObj->AddScript(new CPlayerScript);

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
		CRenderMgr::GetInst()->RegisterCamera(Cam, eCAMERA_INDEX::MAIN);
		pObj->Camera()->SetProjType(ePROJ_TYPE::ORTHOGRAPHY);
		pObj->AddComponent(new CTransform);
		//pObj->AddScript(new CCameraMoveScript);

		SpawnGameObject(pObj, Vec3(0.f, 0.f, -100.f), 1);
		//m_pCurLevel->AddGameObject(pObj, 1);
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

		pPrefab->AddScript(new CBulletScript);

		pPrefab->AddComponent(new CTransform);

		Ptr<CPrefab> pResPrefab = new CPrefab;
		pResPrefab->RegisterPrefab(pPrefab);

		CResMgr::GetInst()->AddRes("Bullet", pResPrefab);
	}
}

void CLevelMgr::tick()
{
	m_pCurLevel->tick();
	m_pCurLevel->finaltick();
}