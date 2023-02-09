#include "pch.h"

#include "CEditorObjMgr.h"

#include <Engine/CGameObject.h>

#include <Engine/CTimeMgr.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CResMgr.h>
#include <Engine/CMeshRender.h>
#include <Engine/CCamera.h>
#include <Engine/CTransform.h>
#include <Engine/CCameraMoveScript.h>

#define DEBUG_MAT_WVP MAT_0
#define DEBUG_VEC4_COLOR VEC4_0

CEditorObjMgr::CEditorObjMgr()
	: m_arrDebugShape{}
	, m_pEditorCam()
{
}
CEditorObjMgr::~CEditorObjMgr()
{
	for (int i = 0; i < (int)eSHAPE_TYPE::END; ++i)
	{
		DESTRUCTOR_DELETE(m_arrDebugShape[i]);
	}

	DESTRUCTOR_DELETE(m_pEditorCam);
}


void CEditorObjMgr::init()
{
	CreateDebugShape();

	CreateEditorCamera();
}

void CEditorObjMgr::progress()
{
	tick();
	render();
}

//Engine으로부터 디버그에 사용할 오브젝트들 정보를 받아오기
void CEditorObjMgr::tick()
{
	//이 메소드의 호출 시점은 모든 게임의 메인 렌더링과정이 종료된 이후이다. 
	


	//CRenderMgr로부터 이번 프레임에 그릴 디버그 쉐이프 목록을 받아온다.
	CRenderMgr::GetInst()->UpdateDebugShapeRender(m_vecDebugShapeInfo);

	float DT = DELTA_TIME;

	//여기선 단순히 시간만 뺴 준다. render()에서 그리고 나서 음수이면 제거함.
	size_t size = m_vecDebugShapeInfo.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecDebugShapeInfo[i].fLifeSpan -= DT;
	}

	m_pEditorCam->tick();
	m_pEditorCam->finaltick();
}

//받아온 오브젝트를 통해서 디버그 정보를 사각형으로 그리기
void CEditorObjMgr::render()
{

	//메인 카메라의 View Projection  행렬을 가져온다.
	const Matrix& matVP = CRenderMgr::GetInst()->GetCurCamera()->GetViewProjMatrix();

	size_t size = m_vecDebugShapeInfo.size();
	for (size_t i = 0; i < size; ++i)
	{
		switch (m_vecDebugShapeInfo[i].eShape)
		{
		case eSHAPE_TYPE::RECT:
		{
			Ptr<CMaterial> pMtrl = m_arrDebugShape[(int)eSHAPE_TYPE::RECT]->MeshRender()->GetMaterial();
			//월드행렬 전달.

			Matrix matWVP = m_vecDebugShapeInfo[i].matWorld * matVP;
			pMtrl->SetScalarParam(DEBUG_MAT_WVP, matWVP.m);
			pMtrl->SetScalarParam(DEBUG_VEC4_COLOR, m_vecDebugShapeInfo[i].vColor);

			//레이어에 속해서 게임 내에서 돌아가는 게임오브젝트가 아니므로 강제로 render()를 호출해야 한다.
			m_arrDebugShape[(int)eSHAPE_TYPE::RECT]->render();

			break;
		}
		case eSHAPE_TYPE::CIRCLE:
		{
			Ptr<CMaterial> pMtrl = m_arrDebugShape[(int)eSHAPE_TYPE::CIRCLE]->MeshRender()->GetMaterial();
			//월드행렬 전달.
			Matrix matWVP = m_vecDebugShapeInfo[i].matWorld * matVP;
			pMtrl->SetScalarParam(DEBUG_MAT_WVP, matWVP.m);
			pMtrl->SetScalarParam(DEBUG_VEC4_COLOR, m_vecDebugShapeInfo[i].vColor);

			m_arrDebugShape[(int)eSHAPE_TYPE::CIRCLE]->render();
			break;
		}
		case eSHAPE_TYPE::CUBE:
			break;
		case eSHAPE_TYPE::SPHERE:
			break;
		case eSHAPE_TYPE::END:
			break;
		default:
			break;
		}
	}

	//남은 시간이 음수가 된 디버그 쉐이프에 대해 삭제 처리를 진행
	m_vecDebugShapeInfo.erase(
		std::remove_if(m_vecDebugShapeInfo.begin(), m_vecDebugShapeInfo.end(),
			[](const tDebugShapeInfo& _Info)->bool
			{
				return (_Info.fLifeSpan < 0.f);
			})
		, m_vecDebugShapeInfo.end());
}

void CEditorObjMgr::CreateDebugShape()
{
	Ptr<CMaterial> pDebugMtrl = CResMgr::GetInst()->FindRes<CMaterial>("DebugMtrl");


	for (int i = 0; i < (int)eSHAPE_TYPE::END; ++i)
	{
		m_arrDebugShape[i] = new CGameObject;

		switch ((eSHAPE_TYPE)i)
		{
		case eSHAPE_TYPE::RECT:
		{
			CMeshRender* pMesh = new CMeshRender;
			//월드행렬을 직접 받아서 쉐이더에 보낼 것이기 떄문에 Transform은 필요하지 않음.
			Ptr<CMesh> pDebugMesh = CResMgr::GetInst()->FindRes<CMesh>("RectMesh_Debug");
			pMesh->SetMesh(pDebugMesh);
			pMesh->SetMaterial(pDebugMtrl);
			m_arrDebugShape[i]->AddComponent(pMesh);

			break;
		}

		case eSHAPE_TYPE::CIRCLE:
		{
			CMeshRender* pMesh = new CMeshRender;
			Ptr<CMesh> pDebugMesh = CResMgr::GetInst()->FindRes<CMesh>("CircleMesh_Debug");
			pMesh->SetMesh(pDebugMesh);
			pMesh->SetMaterial(pDebugMtrl);
			m_arrDebugShape[i]->AddComponent(pMesh);
			break;
		}
		case eSHAPE_TYPE::CUBE:
		{
			break;
		}
		case eSHAPE_TYPE::SPHERE:
		{
			break;
		}
		default:
			break;
		}
	}
}

void CEditorObjMgr::CreateEditorCamera()
{
	m_pEditorCam = new CGameObject;

	CCamera* pCam = new CCamera;
	m_pEditorCam->AddComponent(pCam);
	pCam->SetLayerFlag(UINT32_MAX);

	CTransform* pTransform = new CTransform;
	m_pEditorCam->AddComponent(pTransform);
	pTransform->SetRelativePos(Vec3(0.f, 0.f, -100.f));

	m_pEditorCam->AddScript(new CCameraMoveScript);

	CRenderMgr::GetInst()->SetEditorCam(m_pEditorCam->Camera());
}
