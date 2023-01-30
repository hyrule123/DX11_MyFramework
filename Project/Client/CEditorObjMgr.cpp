#include "pch.h"

#include "CEditorObjMgr.h"

#include <Engine/CGameObject.h>

#include <Engine/CTimeMgr.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CResMgr.h>
#include <Engine/CMeshRender.h>
#include <Engine/CCamera.h>

#define DEBUG_MAT_WVP MAT_0
#define DEBUG_VEC4_COLOR VEC4_0

CEditorObjMgr::CEditorObjMgr()
	: m_arrDebugShape{}
{
}
CEditorObjMgr::~CEditorObjMgr()
{
	for (int i = 0; i < eSHAPE_END; ++i)
	{
		SAFE_DELETE(m_arrDebugShape[i])
	}
}


void CEditorObjMgr::init()
{
	Ptr<CMaterial> pDebugMtrl = CResMgr::GetInst()->FindRes<CMaterial>("DebugMtrl");


	for (int i = 0; i < eSHAPE_END; ++i)
	{
		m_arrDebugShape[i] = new CGameObject;

		switch ((eSHAPE_TYPE)i)
		{
		case eSHAPE_RECT:
		{
			CMeshRender* pMesh = new CMeshRender;
			//��������� ���� �޾Ƽ� ���̴��� ���� ���̱� ������ Transform�� �ʿ����� ����.
			Ptr<CMesh> pDebugMesh = CResMgr::GetInst()->FindRes<CMesh>("RectMesh_Debug");
			pMesh->SetMesh(pDebugMesh);
			pMesh->SetMaterial(pDebugMtrl);
			m_arrDebugShape[i]->AddComponent(pMesh);

			 break;
		}

		case eSHAPE_CIRCLE:
		{
			CMeshRender* pMesh = new CMeshRender;
			Ptr<CMesh> pDebugMesh = CResMgr::GetInst()->FindRes<CMesh>("CircleMesh_Debug");
			pMesh->SetMesh(pDebugMesh);
			pMesh->SetMaterial(pDebugMtrl);
			m_arrDebugShape[i]->AddComponent(pMesh);
			break;
		}
		case eSHAPE_CUBE:
		{
			break;
		}
		case eSHAPE_SPHERE:
		{
			break;
		}
		default:
			break;
		}
	}
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
}

//�޾ƿ� ������Ʈ�� ���ؼ� ����� ������ �簢������ �׸���
void CEditorObjMgr::render()
{

	//���� ī�޶��� View Projection  ����� �����´�.
	const Matrix& matVP = CRenderMgr::GetInst()->GetCamera(eCAMIDX_MAIN)->GetViewProjMatrix();

	size_t size = m_vecDebugShapeInfo.size();
	for (size_t i = 0; i < size; ++i)
	{
		switch (m_vecDebugShapeInfo[i].eShape)
		{
		case eSHAPE_RECT:
		{
			Ptr<CMaterial> pMtrl = m_arrDebugShape[eSHAPE_RECT]->MeshRender()->GetMaterial();
			//������� ����.

			Matrix matWVP = m_vecDebugShapeInfo[i].matWorld * matVP;
			matWVP = matWVP.Transpose();
			pMtrl->SetScalarParam(DEBUG_MAT_WVP, matWVP.m);
			pMtrl->SetScalarParam(DEBUG_VEC4_COLOR, m_vecDebugShapeInfo[i].vColor);

			//���̾ ���ؼ� ���� ������ ���ư��� ���ӿ�����Ʈ�� �ƴϹǷ� ������ render()�� ȣ���ؾ� �Ѵ�.
			m_arrDebugShape[eSHAPE_RECT]->render();

			break;
		}
		case eSHAPE_CIRCLE:
		{
			Ptr<CMaterial> pMtrl = m_arrDebugShape[eSHAPE_CIRCLE]->MeshRender()->GetMaterial();
			//������� ����.
			Matrix matWVP = m_vecDebugShapeInfo[i].matWorld * matVP;
			matWVP = matWVP.Transpose();
			pMtrl->SetScalarParam(DEBUG_MAT_WVP, matWVP.m);
			pMtrl->SetScalarParam(DEBUG_VEC4_COLOR, m_vecDebugShapeInfo[i].vColor);

			m_arrDebugShape[eSHAPE_CIRCLE]->render();
			break;
		}
		case eSHAPE_CUBE:
			break;
		case eSHAPE_SPHERE:
			break;
		case eSHAPE_END:
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
