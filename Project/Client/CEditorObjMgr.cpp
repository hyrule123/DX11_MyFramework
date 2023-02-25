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

#include <Engine/strKeys.h>

#include <Engine/Shader_Debug_0_Header.hlsli>


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
	CCamera* pCam = CRenderMgr::GetInst()->GetCurCamera();
	//ī�޶��� �����͸� ���ε�
	pCam->UploadData();


	size_t size = m_vecDebugShapeInfo.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecDebugShapeInfo[i].fLifeSpan -= DELTA_TIME;


		//������� ����.
		tMtrlScalarData MtrlData = {};
		Ptr<CMaterial> pMtrl = nullptr;

		switch ((eSHAPE_TYPE)m_vecDebugShapeInfo[i].eShapeType)
		{
		case eSHAPE_TYPE::RECT:
		{
			pMtrl = m_arrDebugShape[(int)eSHAPE_TYPE::RECT]->MeshRender()->GetCurMaterial();


			//m_arrDebugShape[(int)eSHAPE_TYPE::RECT]->SetMtrlScalarParam(MTRL_SCALAR_DEBUG_MAT_WORLD, m_vecDebugShapeInfo[i].matWorld.m);

			//m_arrDebugShape[(int)eSHAPE_TYPE::RECT]->SetMtrlScalarParam(MTRL_SCALAR_DEBUG_VEC4_COLOR, &(m_vecDebugShapeInfo[i].vColor));

			//���̾ ���ؼ� ���� ������ ���ư��� ���ӿ�����Ʈ�� �ƴϹǷ� ������ render()�� ȣ���ؾ� �Ѵ�.
			/*m_arrDebugShape[(int)eSHAPE_TYPE::RECT]->render();*/

			break;
		}
		case eSHAPE_TYPE::CIRCLE:
		{
			pMtrl = m_arrDebugShape[(int)eSHAPE_TYPE::CIRCLE]->MeshRender()->GetCurMaterial();


			//������� ����.
			//const Matrix& matWorld = m_vecDebugShapeInfo[i].matWorld;

			//m_arrDebugShape[(int)eSHAPE_TYPE::CIRCLE]->SetMtrlScalarParam(MTRL_SCALAR_DEBUG_MAT_WORLD, m_vecDebugShapeInfo[i].matWorld.m);

			//m_arrDebugShape[(int)eSHAPE_TYPE::CIRCLE]->SetMtrlScalarParam(MTRL_SCALAR_DEBUG_VEC4_COLOR, &(m_vecDebugShapeInfo[i].vColor));

			////���̾ ���ؼ� ���� ������ ���ư��� ���ӿ�����Ʈ�� �ƴϹǷ� ������ render()�� ȣ���ؾ� �Ѵ�.
			//m_arrDebugShape[(int)eSHAPE_TYPE::CIRCLE]->render();
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



		//MAT_WORLD = 1��
		MtrlData.MAT_1 = m_vecDebugShapeInfo[i].matWorld;

		//MAT_COLOR = 0��
		MtrlData.VEC4_0 = m_vecDebugShapeInfo[i].vColor;

		//���� ������ ������ ����Ѵ�.
		pMtrl->AddMtrlScalarData(MtrlData);
	}

	for (int i = 0; i < (int)eSHAPE_TYPE::END; ++i)
	{
		CRenderComponent* pRenderCom = m_arrDebugShape[i]->GetRenderComponent();
		if (nullptr == pRenderCom)
			continue;

		CMaterial* pMtrl = pRenderCom->GetCurMaterial().Get();
		pMtrl->BindData();
		pRenderCom->GetMesh()->renderInstanced(pMtrl->GetInstancingCount());
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
	

	for (int i = 0; i < (int)eSHAPE_TYPE::END; ++i)
	{
		m_arrDebugShape[i] = new CGameObject;

		switch ((eSHAPE_TYPE)i)
		{
		case eSHAPE_TYPE::RECT:
		{
			CMeshRender* pMesh = new CMeshRender;
			//��������� ���� �޾Ƽ� ���̴��� ���� ���̱� ������ Transform�� �ʿ����� ����.
			Ptr<CMesh> pDebugMesh = CResMgr::GetInst()->FindRes<CMesh>(RESOURCE::MESH::DEBUG_RECT);
			Ptr<CMaterial> pDebugMtrl = CResMgr::GetInst()->FindRes<CMaterial>(RESOURCE::MATERIAL::DEBUG_RECT);
			pMesh->SetMesh(pDebugMesh);
			pMesh->SetMaterial(pDebugMtrl);
			m_arrDebugShape[i]->AddComponent(pMesh);

			break;
		}

		case eSHAPE_TYPE::CIRCLE:
		{
			CMeshRender* pMesh = new CMeshRender;
			Ptr<CMesh> pDebugMesh = CResMgr::GetInst()->FindRes<CMesh>(RESOURCE::MESH::DEBUG_CIRCLE);
			Ptr<CMaterial> pDebugMtrl = CResMgr::GetInst()->FindRes<CMaterial>(RESOURCE::MATERIAL::DEBUG_CIRCLE);
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
