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

CLevelMgr::CLevelMgr()
	: m_pCurLevel(nullptr)
{
}

CLevelMgr::~CLevelMgr()
{
	if (nullptr != m_pCurLevel)
		delete m_pCurLevel;
}


void CLevelMgr::init()
{
	m_pCurLevel = new CLevel;

	Ptr<CMesh> CircleMesh = CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh");
	Ptr<CTexture> PlayerTex = CResMgr::GetInst()->FindRes<CTexture>(L"PlayerTex");
	Vec4 ColorKey(1.f, 1.f, 1.f, 1.f);

	// 오브젝트 생성
	CGameObject* pPlayer = new CGameObject;
	pPlayer->SetName(L"Player");
	pPlayer->AddComponent(new CTransform);
	pPlayer->Transform()->SetRelativePosZ(50.f);
	pPlayer->Transform()->SetRelativeRot(0.f, 0.f, -XM_PI / 2.f);
	pPlayer->Transform()->SetSize(Vec3(100.f, 100.f, 1.f));
	pPlayer->AddComponent(new CMeshRender);
	pPlayer->AddScript(new CPlayerScript);

	Ptr<CMaterial> PlayerMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"std2DMtrl");
	PlayerMtrl->SetTexParam(eTEX_0, PlayerTex);
	PlayerMtrl->SetScalarParam((eSCALAR_PARAM)COLOR_KEY, ColorKey);

	pPlayer->MeshRender()->SetMesh(CircleMesh);
	pPlayer->MeshRender()->SetMaterial(PlayerMtrl);
	m_pCurLevel->AddGameObject(pPlayer, 1);

	// Test Object 1
	CGameObject* pTestObj1 = new CGameObject;
	pTestObj1->SetName(L"Test Object");
	pTestObj1->AddComponent(new CTransform);
	pTestObj1->Transform()->SetRelativePos(100.f, 0.f, 10.f);
	//pTestObj1->Transform()->SetRotInheritance(false);
	//pTestObj1->Transform()->SetScaleInheritance(false);
	pTestObj1->Transform()->SetSize(Vec3(100.f, 100.f, 1.f));
	pTestObj1->AddComponent(new CMeshRender);
	pTestObj1->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	Ptr<CMaterial> TestMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
	TestMtrl->SetScalarParam((eSCALAR_PARAM)COLOR_KEY, ColorKey);
	pTestObj1->MeshRender()->SetMaterial(TestMtrl);
	pPlayer->AddChild(pTestObj1);
	m_pCurLevel->AddGameObject(pTestObj1, 1);


	// Test Object 2
	CGameObject* pTestObj2 = new CGameObject;
	pTestObj2->SetName(L"Test Object");
	pTestObj2->AddComponent(new CTransform);
	pTestObj2->Transform()->SetRelativePos(0.f, 100.f, 10.f);
	pTestObj2->Transform()->SetRotInheritance(false);
	pTestObj2->Transform()->SetScaleInheritance(false);
	pTestObj2->Transform()->SetSize(Vec3(100.f, 100.f, 1.f));
	pTestObj2->AddComponent(new CMeshRender);
	pTestObj2->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	TestMtrl->SetScalarParam((eSCALAR_PARAM)COLOR_KEY, ColorKey);
	pTestObj2->MeshRender()->SetMaterial(TestMtrl);
	pTestObj1->AddChild(pTestObj2);
	pTestObj2->AddScript(new CTestObjScript);
	m_pCurLevel->AddGameObject(pTestObj2, 1);

	{
		// Test Object 3
		CGameObject* pTestObj3 = new CGameObject;
		pTestObj3->SetName(L"Test Object");
		pTestObj3->AddComponent(new CTransform);
		pTestObj3->Transform()->SetRelativePos(0.f, 100.f, 10.f);
		pTestObj3->Transform()->SetSize(Vec3(100.f, 100.f, 1.f));
		pTestObj3->AddComponent(new CMeshRender);
		pTestObj3->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Ptr<CMaterial> TestMtrl3 = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
		TestMtrl->SetScalarParam((eSCALAR_PARAM)COLOR_KEY, ColorKey);
		pTestObj3->MeshRender()->SetMaterial(TestMtrl);
		m_pCurLevel->AddGameObject(pTestObj3, 1);

		pTestObj2->AddChild(pTestObj3);
	}


	{
		// Camera
		CGameObject* pObj = new CGameObject;
		pObj->SetName(L"Camera");
		CCamera* Cam = new CCamera;
		pObj->AddComponent(Cam);
		CRenderMgr::GetInst()->RegisterCamera(Cam, eCAMIDX_MAIN);
		pObj->Camera()->SetProjType(ePROJ_TYPE::ORTHOGRAPHY);
		pObj->AddComponent(new CTransform);
		pObj->AddScript(new CCameraMoveScript);
		
		m_pCurLevel->AddGameObject(pObj, 1);

		pPlayer->AddChild(pObj);
	}

	{//Prefab
		CGameObject* pPrefab = new CGameObject;

		pPrefab->AddComponent(new CMeshRender);
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
		Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(L"HOS");

		pMtrl->SetTexParam(eTEX_0, pTex);
		Ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh");
		pPrefab->MeshRender()->SetMaterial(pMtrl);
		pPrefab->MeshRender()->SetMesh(pMesh);

		pPrefab->AddScript(new CBulletScript);

		pPrefab->AddComponent(new CTransform);

		Ptr<CPrefab> pResPrefab = new CPrefab;
		pResPrefab->RegisterPrefab(pPrefab);

		CResMgr::GetInst()->AddRes(L"Bullet", pResPrefab);
	}
}

void CLevelMgr::tick()
{
	m_pCurLevel->tick();
	m_pCurLevel->finaltick();
}