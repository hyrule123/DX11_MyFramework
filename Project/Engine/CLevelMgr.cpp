#include "pch.h"
#include "CLevelMgr.h"

#include "CLevel.h"
#include "CLayer.h"

#include "CGameObject.h"
#include "components.h"
#include "CMeshRender.h"
#include "CPlayerScript.h"

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

	Ptr<CMaterial> Mtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"std2DMtrl");
	Mtrl->SetTexParam(eTEX_0, PlayerTex);
	Mtrl->SetScalarParam((eSCALAR_PARAM)COLOR_KEY, ColorKey);

	pPlayer->MeshRender()->SetMesh(CircleMesh);
	pPlayer->MeshRender()->SetMaterial(Mtrl);
	m_pCurLevel->AddGameObject(pPlayer, 1);

	// Test Object 1
	CGameObject* pChild = new CGameObject;
	pChild->SetName(L"Test Object");
	pChild->AddComponent(new CTransform);
	pChild->Transform()->SetRelativePos(100.f, 0.f, 10.f);
	//pChild->Transform()->SetRotInheritance(false);
	//pChild->Transform()->SetScaleInheritance(false);
	pChild->Transform()->SetSize(Vec3(100.f, 100.f, 1.f));
	pChild->AddComponent(new CMeshRender);
	pChild->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	Ptr<CMaterial> TestMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
	TestMtrl->SetScalarParam((eSCALAR_PARAM)COLOR_KEY, ColorKey);
	pChild->MeshRender()->SetMaterial(TestMtrl);
	pPlayer->AddChild(pChild);
	m_pCurLevel->AddGameObject(pChild, 1);

	{
		// Test Object 2
		CGameObject* pObj = new CGameObject;
		pObj->SetName(L"Test Object");
		pObj->AddComponent(new CTransform);
		pObj->Transform()->SetRelativePos(0.f, 100.f, 10.f);
		pObj->Transform()->SetRotInheritance(false);
		pObj->Transform()->SetScaleInheritance(false);
		pObj->Transform()->SetSize(Vec3(100.f, 100.f, 1.f));
		pObj->AddComponent(new CMeshRender);
		pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Ptr<CMaterial> TestMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
		TestMtrl->SetScalarParam((eSCALAR_PARAM)COLOR_KEY, ColorKey);
		pObj->MeshRender()->SetMaterial(TestMtrl);
		pChild->AddChild(pObj);
		m_pCurLevel->AddGameObject(pObj, 1);
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
	}

}

void CLevelMgr::tick()
{
	m_pCurLevel->tick();
	m_pCurLevel->finaltick();
}