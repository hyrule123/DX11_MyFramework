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

#define DEBUG_MAT_WVP eMTRLDATA_PARAM_SCALAR::MAT_0
#define DEBUG_VEC4_COLOR eMTRLDATA_PARAM_SCALAR::VEC4_0

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

//Engine���κ��� ����׿� ����� ������Ʈ�� ������ �޾ƿ���
void CEditorObjMgr::tick()
{
	//�� �޼ҵ��� ȣ�� ������ ��� ������ ���� ������������ ����� �����̴�. 
	


	//CRenderMgr�κ��� �̹� �����ӿ� �׸� ����� ������ ����� �޾ƿ´�.
	CRenderMgr::GetInst()->UpdateDebugShapeRender(m_vecDebugShapeInfo);

	float DT = DELTA_TIME;

	//���⼱ �ܼ��� �ð��� �� �ش�. render()���� �׸��� ���� �����̸� ������.
	size_t size = m_vecDebugShapeInfo.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecDebugShapeInfo[i].fLifeSpan -= DT;
	}

	m_pEditorCam->tick();
	m_pEditorCam->finaltick();
}

//�޾ƿ� ������Ʈ�� ���ؼ� ����� ������ �簢������ �׸���
void CEditorObjMgr::render()
{

	//���� ī�޶��� View Projection  ����� �����´�.
	const Matrix& matVP = CRenderMgr::GetInst()->GetCurCamera()->GetViewProjMatrix();

	size_t size = m_vecDebugShapeInfo.size();
	for (size_t i = 0; i < size; ++i)
	{
		switch (m_vecDebugShapeInfo[i].eShape)
		{
		case eSHAPE_TYPE::RECT:
		{
			Ptr<CMaterial> pMtrl = m_arrDebugShape[(int)eSHAPE_TYPE::RECT]->MeshRender()->GetMaterial();
			//������� ����.

			Matrix matWVP = m_vecDebugShapeInfo[i].matWorld * matVP;
			pMtrl->SetScalarParam(DEBUG_MAT_WVP, matWVP.m);
			pMtrl->SetScalarParam(DEBUG_VEC4_COLOR, m_vecDebugShapeInfo[i].vColor);

			//���̾ ���ؼ� ���� ������ ���ư��� ���ӿ�����Ʈ�� �ƴϹǷ� ������ render()�� ȣ���ؾ� �Ѵ�.
			m_arrDebugShape[(int)eSHAPE_TYPE::RECT]->render();

			break;
		}
		case eSHAPE_TYPE::CIRCLE:
		{
			Ptr<CMaterial> pMtrl = m_arrDebugShape[(int)eSHAPE_TYPE::CIRCLE]->MeshRender()->GetMaterial();
			//������� ����.
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

	//���� �ð��� ������ �� ����� �������� ���� ���� ó���� ����
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
			//��������� ���� �޾Ƽ� ���̴��� ���� ���̱� ������ Transform�� �ʿ����� ����.
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
