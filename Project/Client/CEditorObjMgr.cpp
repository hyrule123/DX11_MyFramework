#include "pch.h"

#include "CEditorObjMgr.h"

#include <Engine/CGameObject.h>

#include <Engine/CTimeMgr.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CResMgr.h>
#include <Engine/CMeshRender.h>

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
	Ptr<CMaterial> pDebugMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"DebugMtrl");


	for (int i = 0; i < eSHAPE_END; ++i)
	{
		m_arrDebugShape[i] = new CGameObject;

		switch ((eSHAPE_TYPE)i)
		{
		case eSHAPE_RECT:
		{
			CMeshRender* pMesh = new CMeshRender;
			//��������� ���� �޾Ƽ� ���̴��� ���� ���̱� ������ Transform�� �ʿ����� ����.
			Ptr<CMesh> pDebugMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_Debug");
			pMesh->SetMesh(pDebugMesh);
			pMesh->SetMaterial(pDebugMtrl);
			m_arrDebugShape[i]->AddComponent(pMesh);

			 break;
		}

		case eSHAPE_CIRCLE:
		{
			CMeshRender* pMesh = new CMeshRender;
			Ptr<CMesh> pDebugMesh = CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh_Debug");
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
	CRenderMgr::GetInst()->UpdateDebugShapeRender(m_listDebugShapeInfo);

	float DT = DELTA_TIME;

	//���⼱ �ܼ��� �ð��� �� �ش�. render()���� �׸��� ���� �����̸� ������.
	for (auto& iter : m_listDebugShapeInfo)
	{
		iter.fLifeSpan -= DT;
	}
}

//�޾ƿ� ������Ʈ�� ���ؼ� ����� ������ �簢������ �׸���
void CEditorObjMgr::render()
{
	auto iter = m_listDebugShapeInfo.begin();
	const auto& iterEnd = m_listDebugShapeInfo.end();
	while (iter != iterEnd)
	{
		switch (iter->eShape)
		{
		case eSHAPE_RECT:
		{
			Ptr<CMaterial> pMtrl = m_arrDebugShape[eSHAPE_RECT]->MeshRender()->GetMaterial();
			//������� ����.
			pMtrl->SetScalarParam(DEBUG_MAT_WVP, iter->matWVP.m);
			pMtrl->SetScalarParam(DEBUG_VEC4_COLOR, iter->vColor);

			//���̾ ���ؼ� ���� ������ ���ư��� ���ӿ�����Ʈ�� �ƴϹǷ� ������ render()�� ȣ���ؾ� �Ѵ�.
			m_arrDebugShape[eSHAPE_RECT]->render();

			break;
		}
		case eSHAPE_CIRCLE:
		{
			Ptr<CMaterial> pMtrl = m_arrDebugShape[eSHAPE_CIRCLE]->MeshRender()->GetMaterial();
			//������� ����.
			pMtrl->SetScalarParam(DEBUG_MAT_WVP, iter->matWVP.m);
			pMtrl->SetScalarParam(DEBUG_VEC4_COLOR, iter->vColor);

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
			
			//�ð��� �� ���� ��� �ش� iterator�� ����
		if (iter->fLifeSpan < 0.f)
		{
			iter = m_listDebugShapeInfo.erase(iter);
		}
		else
		{
			++iter;
		}
	}

}
