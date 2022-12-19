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

	// 오브젝트 생성
	CGameObject* pObj = new CGameObject;
	pObj->SetName(L"Player");
	pObj->AddComponent(new CTransform);
	pObj->Transform()->SetRelativePosZ(50.f);
	pObj->Transform()->SetRelativeRot(0.f, 0.f, -XM_PI / 2.f);
	pObj->Transform()->SetRelativeScale(100.f, 100.f, 1.f);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CPlayerScript);

	Ptr<CMesh> CircleMesh = CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh");
	Ptr<CMaterial> TestMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
	Ptr<CTexture> PlayerTex = CResMgr::GetInst()->FindRes<CTexture>(L"PlayerTex");

	TestMtrl->SetTexParam(TEX_0, PlayerTex);

	pObj->MeshRender()->SetMesh(CircleMesh);
	pObj->MeshRender()->SetMaterial(TestMtrl);
	m_pCurLevel->AddGameObject(pObj, 0);
	pObj = nullptr;

	// Test Object
	pObj = new CGameObject;
	pObj->SetName(L"Test Object");
	pObj->AddComponent(new CTransform);
	pObj->Transform()->SetRelativePosZ(100.f);
	pObj->Transform()->SetRelativeScale(100.f, 100.f, 1.f);
	pObj->AddComponent(new CMeshRender);	
	pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(TestMtrl);
	m_pCurLevel->AddGameObject(pObj, 1);
	pObj = nullptr;

	// Camera
	pObj = new CGameObject;
	pObj->SetName(L"Camera");
	pObj->AddComponent(new CCamera);
	pObj->Camera()->SetProjType(ePROJ_TYPE::ORTHOGRAPHY);

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CCameraMoveScript);
	m_pCurLevel->AddGameObject(pObj, 1);

}

void CLevelMgr::tick()
{
	m_pCurLevel->tick();
	m_pCurLevel->finaltick();
}

void CLevelMgr::render()
{
	m_pCurLevel->render();
}
