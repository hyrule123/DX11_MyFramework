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
	{
		CGameObject* pObj = new CGameObject;
		pObj->SetName(L"Player");
		pObj->AddComponent(new CTransform);
		pObj->Transform()->SetRelativePosZ(50.f);
		pObj->Transform()->SetRelativeRot(0.f, 0.f, -XM_PI / 2.f);
		pObj->Transform()->SetRelativeScale(100.f, 100.f, 1.f);
		pObj->AddComponent(new CMeshRender);
		pObj->AddComponent(new CPlayerScript);

		Ptr<CMaterial> Mtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"std2DMtrl");
		Mtrl->SetTexParam(TEX_0, PlayerTex);

		
		Mtrl->SetScalarParam((eSCALAR_PARAM)COLOR_KEY, ColorKey);

		pObj->MeshRender()->SetMesh(CircleMesh);
		pObj->MeshRender()->SetMaterial(Mtrl);
		m_pCurLevel->AddGameObject(pObj, 1);
	}

	{
		// Test Object
		CGameObject* pObj = new CGameObject;
		pObj->SetName(L"Test Object");
		pObj->AddComponent(new CTransform);
		pObj->Transform()->SetRelativePosZ(20.f);
		pObj->Transform()->SetRelativeScale(100.f, 100.f, 1.f);
		pObj->AddComponent(new CMeshRender);
		pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Ptr<CMaterial> TestMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
		TestMtrl->SetScalarParam((eSCALAR_PARAM)COLOR_KEY, ColorKey);
		pObj->MeshRender()->SetMaterial(TestMtrl);
		m_pCurLevel->AddGameObject(pObj, 1);
	}

	{
		// Camera
		CGameObject* pObj = new CGameObject;
		pObj->SetName(L"Camera");
		CCamera* Cam = new CCamera;
		pObj->AddComponent(Cam);
		CRenderMgr::GetInst()->RegisterCamera(Cam, 0);
		pObj->Camera()->SetProjType(ePROJ_TYPE::ORTHOGRAPHY);
		pObj->AddComponent(new CTransform);
		pObj->AddComponent(new CCameraMoveScript);
		
		m_pCurLevel->AddGameObject(pObj, 1);
	}

}

void CLevelMgr::tick()
{
	m_pCurLevel->tick();
	m_pCurLevel->finaltick();
}