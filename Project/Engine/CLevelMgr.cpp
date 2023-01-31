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

//�浹 ���̾� ��� �� üũ��
#include "CCollisionMgr.h"

//Ÿ�ϸ�
#include "CTilemap.h"



CLevelMgr::CLevelMgr()
	: m_pCurLevel(nullptr)
{
}

CLevelMgr::~CLevelMgr()
{
	if (nullptr != m_pCurLevel)
		delete m_pCurLevel;
}


CGameObject* CLevelMgr::FindObjectByName(const wstring& _Name)
{
	return m_pCurLevel->FindObjectByName(_Name);
}

void CLevelMgr::FindObjectALLByName(const wstring& _Name, vector<CGameObject*>& _vecObj)
{
	m_pCurLevel->FindObjectALLByName(_Name, _vecObj);
}

void CLevelMgr::init()
{
	m_pCurLevel = new CLevel;

	Ptr<CMesh> CircleMesh = CResMgr::GetInst()->FindRes<CMesh>("CircleMesh");
	Ptr<CTexture> Fighter = CResMgr::GetInst()->FindRes<CTexture>("Fighter");
	Vec4 ColorKey(1.f, 1.f, 1.f, 1.f);

	// ������Ʈ ����
	CGameObject* pPlayer = new CGameObject;
	pPlayer->SetName(L"Player");
	pPlayer->AddComponent(new CTransform);
	//pPlayer->Transform()->SetRelativePosZ(50.f);
	pPlayer->Transform()->SetRelativeRot(0.f, 0.f, -XM_PI / 2.f);
	pPlayer->Transform()->SetSize(Vec3(100.f, 100.f, 1.f));
	pPlayer->AddComponent(new CMeshRender);
	pPlayer->AddScript(new CPlayerScript);

	Ptr<CMaterial> PlayerMtrl = CResMgr::GetInst()->FindRes<CMaterial>("std2DMtrl");
	PlayerMtrl->SetTexParam(eTEX_0, Fighter);
	PlayerMtrl->SetScalarParam((eSCALAR_PARAM)COLOR_KEY, ColorKey);

	pPlayer->MeshRender()->SetMesh(CircleMesh);
	pPlayer->MeshRender()->SetMaterial(PlayerMtrl);

	pPlayer->AddComponent(new CLight2D);
	pPlayer->Light2D()->SetLightType(eLIGHT_TYPE::eLIGHT_POINT);

	pPlayer->AddComponent(new CCollider2D_OBB);

	::SpawnGameObject(pPlayer, Vec3(0.f, 0.f, 10.f), 1);
	//m_pCurLevel->AddGameObject(pPlayer, 1);

	// Test Object 1
	//CGameObject* pTestObj1 = new CGameObject;
	//pTestObj1->SetName(L"Test Object");
	//pTestObj1->AddComponent(new CTransform);
	//pTestObj1->Transform()->SetSize(Vec3(100.f, 100.f, 1.f));
	//pTestObj1->AddComponent(new CMeshRender);
	//pTestObj1->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>("RectMesh"));
	//Ptr<CMaterial> TestMtrl = CResMgr::GetInst()->FindRes<CMaterial>("TestMtrl");
	//TestMtrl->SetTexParam(eTEX_0, Fighter);
	//TestMtrl->SetScalarParam((eSCALAR_PARAM)COLOR_KEY, ColorKey);
	//pTestObj1->MeshRender()->SetMaterial(TestMtrl);

	//::SpawnGameObject(pTestObj1, Vec3(100.f, 0.f, 10.f), 1);
	//::AddChild(pPlayer, pTestObj1);


	//SpawnGameObject(pPlayer, Vec3(100.f, 0.f, 10.f), 1);
	//m_pCurLevel->AddGameObject(pTestObj1, 1);


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
	//pLightMtrl->SetScalarParam(COLOR_KEY, Vec4(0.f, 0.f, 0.f, 1.f));
	//pLightMtrl->SetTexParam(eTEX_1, CResMgr::GetInst()->FindRes<CTexture>("beheaded_n"));

	//pTestObj2->MeshRender()->SetMaterial(pLightMtrl);

	//pTestObj2->AddScript(new CTestObjScript);
	//
	//::SpawnGameObject(pTestObj2, Vec3(300.f, 300.f, 10.f), 1);



	//::AddChild(pTestObj1, pTestObj2);
	//pTestObj1->AddChild(pTestObj2);
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
	//		TestMtrl->SetScalarParam((eSCALAR_PARAM)COLOR_KEY, ColorKey);
	//		pTestObj3->MeshRender()->SetMaterial(TestMtrl);
	//		pTestObj3->AddComponent(new CCollider2D_OBB);

	//		//pTestObj3->AddScript(new CTestObjScript);

	//		SpawnGameObject(pTestObj3, Vec3(startxy + 110.f * i, startxy + 110.f * j, 10.f), 1);
	//		//m_pCurLevel->AddGameObject(pTestObj3, 1);
	//	}

	//}


	//// Test Object 3
	//CGameObject* pTestObj3 = new CGameObject;
	//pTestObj3->SetName(L"Test Object");
	//pTestObj3->AddComponent(new CTransform);
	//pTestObj3->Transform()->SetSize(Vec3(100.f, 100.f, 1.f));
	//pTestObj3->AddComponent(new CMeshRender);
	//pTestObj3->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//Ptr<CMaterial> TestMtrl3 = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
	//TestMtrl->SetScalarParam((eSCALAR_PARAM)COLOR_KEY, ColorKey);
	//pTestObj3->MeshRender()->SetMaterial(TestMtrl);
	//pTestObj3->AddComponent(new CCollider2D_OBB);

	//pTestObj3->AddScript(new CTestObjScript);

	//SpawnGameObject(pTestObj3, Vec3(500.f, 500.f, 10.f), 1);
	////m_pCurLevel->AddGameObject(pTestObj3, 1);

	{//Tilemap
		CGameObject* pTilemap = new CGameObject;

		pTilemap->AddComponent(new CTransform);
		pTilemap->AddComponent(new CTilemap);

		pTilemap->Transform()->SetSize(Vec3(500.f, 500.f, 1.f));

		pTilemap->Tilemap()->GetMaterial()->SetTexParam(eTEX_0, CResMgr::GetInst()->FindRes<CTexture>("TileAtlas"));
		pTilemap->Tilemap()->SetSliceSize(Vec2(0.125f, 0.166f));
		pTilemap->Tilemap()->SetTileCount(8, 8);

		::SpawnGameObject(pTilemap, Vec3(0.f, 0.f, 500.f), 1);
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

		SpawnGameObject(pObj, Vec3(0.f, 0.f, -100.f), 1);
		//m_pCurLevel->AddGameObject(pObj, 1);
	}

	{//Prefab
		CGameObject* pPrefab = new CGameObject;

		pPrefab->AddComponent(new CMeshRender);
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>("TestMtrl");
		Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>("HOS");

		pMtrl->SetTexParam(eTEX_0, pTex);
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