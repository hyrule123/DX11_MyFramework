#include "pch.h"

#include "CEditorObjMgr.h"
#include "CImGuiMgr.h"
#include "CUI_Inspector.h"

#include <Engine/CGameObject.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CResMgr.h>
#include <Engine/CMeshRender.h>
#include <Engine/CCamera.h>
#include <Engine/CTransform.h>
#include <Engine/strKeys.h>
#include <Engine/CCollider2D_Point.h>

#include <Engine/Shader_Debug_0_Header.hlsli>

#include <Engine/CScriptHolder.h>
#include <Script/CScript_CameraMove.h>
#include <Script/CScript_MouseCursor.h>



//�׽�Ʈ�� ����
#include "CTestLevel.h"


CEditorObjMgr::CEditorObjMgr()
	: m_arrDebugShape{}
	, m_pEditorCam()
	, m_pMousePicker()
{
}
CEditorObjMgr::~CEditorObjMgr()
{
	for (int i = 0; i < (int)eDEBUGSHAPE_TYPE::END; ++i)
	{
		DESTRUCTOR_DELETE(m_arrDebugShape[i]);
	}

	DESTRUCTOR_DELETE(m_pEditorCam);
	//DESTRUCTOR_DELETE(m_pMousePicker);
}


void CEditorObjMgr::init()
{
	::CreateTestLevel();

	CreateDebugShape();

	CreateEditorCamera();


	{//Ŀ�� ����
		m_pMousePicker = new CGameObject;
		m_pMousePicker->AddComponent(new CTransform);
		m_pMousePicker->AddComponent(new CCollider2D_Point);

		m_pMousePicker->AddScript(new CScript_MouseCursor);

		CScript_MouseCursor* pScript = m_pMousePicker->ScriptHolder()->GetScript<CScript_MouseCursor>();
		pScript->AddFuncLBTNCallback(eKEY_STATE::TAP, std::bind(&CEditorObjMgr::MouseLBTNCallback, this, std::placeholders::_1));

		::SpawnGameObject(m_pMousePicker, Vec3(0.f, 0.f, 0.f), 0);
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


	m_pEditorCam->tick();
	m_pEditorCam->finaltick();
}

//�޾ƿ� ������Ʈ�� ���ؼ� ����� ������ �簢������ �׸���
void CEditorObjMgr::render()
{
	CCamera* pCam = CRenderMgr::GetInst()->GetCurCamera();
	//ī�޶��� �����͸� ���ε�
	


	size_t size = m_vecDebugShapeInfo.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecDebugShapeInfo[i].fLifeSpan -= DELTA_TIME;


		//������� ����.
		tMtrlScalarData MtrlData = {};
		Ptr<CMaterial> pMtrl = nullptr;


		//���⼭ ��������� �ϴ°͵�
//#define MTRL_SCALAR_DEBUG_MAT_VP    MTRLDATA_PARAM_SCALAR(MAT_0)
//#define MTRL_SCALAR_DEBUG_MAT_WORLD MTRLDATA_PARAM_SCALAR(MAT_1)
//
//#define MTRL_SCALAR_DEBUG_INT_SHAPETYPE MTRLDATA_PARAM_SCALAR(INT_0)
//
//#define MTRL_SCALAR_DEBUG_VEC4_COLOR     MTRLDATA_PARAM_SCALAR(VEC4_0)


		switch ((eDEBUGSHAPE_TYPE)m_vecDebugShapeInfo[i].eShapeType)
		{
		case eDEBUGSHAPE_TYPE::RECT:
		{
			pMtrl = m_arrDebugShape[(int)eDEBUGSHAPE_TYPE::RECT]->MeshRender()->GetCurMaterial();

			MtrlData.INT_0 = (int)eDEBUGSHAPE_TYPE::RECT;

			//MAT_WORLD = 1��, VP ����� g_CBuffer_matCam�� �����Ƿ� ���� �������� ����
			MtrlData.MAT_1 = m_vecDebugShapeInfo[i].matWorld;


			//m_arrDebugShape[(int)eDEBUGSHAPE_TYPE::RECT]->SetMtrlScalarParam(MTRL_SCALAR_DEBUG_MAT_WORLD, m_vecDebugShapeInfo[i].matWorld.m);

			//m_arrDebugShape[(int)eDEBUGSHAPE_TYPE::RECT]->SetMtrlScalarParam(MTRL_SCALAR_DEBUG_VEC4_COLOR, &(m_vecDebugShapeInfo[i].vColor));

			//���̾ ���ؼ� ���� ������ ���ư��� ���ӿ�����Ʈ�� �ƴϹǷ� ������ render()�� ȣ���ؾ� �Ѵ�.
			/*m_arrDebugShape[(int)eDEBUGSHAPE_TYPE::RECT]->render();*/

			break;
		}
		case eDEBUGSHAPE_TYPE::CIRCLE:
		{
			pMtrl = m_arrDebugShape[(int)eDEBUGSHAPE_TYPE::CIRCLE]->MeshRender()->GetCurMaterial();

			MtrlData.INT_0 = (int)eDEBUGSHAPE_TYPE::CIRCLE;

			//CIRCLE�� ��� ������ ������ �����Ƿ� ������� ��� WVP ����� ���� ����
			//MAT_WVP = 0��
			MtrlData.MAT_0 = m_vecDebugShapeInfo[i].matWorld * g_matCam[(int)pCam->GetCamIndex()].matVP;

			//������� ����.
			//const Matrix& matWorld = m_vecDebugShapeInfo[i].matWorld;

			//m_arrDebugShape[(int)eDEBUGSHAPE_TYPE::CIRCLE]->SetMtrlScalarParam(MTRL_SCALAR_DEBUG_MAT_WORLD, m_vecDebugShapeInfo[i].matWorld.m);

			//m_arrDebugShape[(int)eDEBUGSHAPE_TYPE::CIRCLE]->SetMtrlScalarParam(MTRL_SCALAR_DEBUG_VEC4_COLOR, &(m_vecDebugShapeInfo[i].vColor));

			////���̾ ���ؼ� ���� ������ ���ư��� ���ӿ�����Ʈ�� �ƴϹǷ� ������ render()�� ȣ���ؾ� �Ѵ�.
			//m_arrDebugShape[(int)eDEBUGSHAPE_TYPE::CIRCLE]->render();
			break;
		}
		case eDEBUGSHAPE_TYPE::CUBE:
			break;
		case eDEBUGSHAPE_TYPE::SPHERE:
			break;
		case eDEBUGSHAPE_TYPE::END:
			break;
		default:
			break;
		}


		//MAT_COLOR = 0��
		MtrlData.VEC4_0 = m_vecDebugShapeInfo[i].vColor;

		//���� ������ ������ ����Ѵ�.
		pMtrl->AddMtrlScalarData(MtrlData);
	}

	for (int i = 0; i < (int)eDEBUGSHAPE_TYPE::END; ++i)
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
	

	for (int i = 0; i < (int)eDEBUGSHAPE_TYPE::END; ++i)
	{
		m_arrDebugShape[i] = new CGameObject;

		switch ((eDEBUGSHAPE_TYPE)i)
		{
		case eDEBUGSHAPE_TYPE::RECT:
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

		case eDEBUGSHAPE_TYPE::CIRCLE:
		{
			CMeshRender* pMesh = new CMeshRender;
			Ptr<CMesh> pDebugMesh = CResMgr::GetInst()->FindRes<CMesh>(RESOURCE::MESH::DEBUG_CIRCLE);
			Ptr<CMaterial> pDebugMtrl = CResMgr::GetInst()->FindRes<CMaterial>(RESOURCE::MATERIAL::DEBUG_CIRCLE);
			pMesh->SetMesh(pDebugMesh);
			pMesh->SetMaterial(pDebugMtrl);
			m_arrDebugShape[i]->AddComponent(pMesh);
			break;
		}
		case eDEBUGSHAPE_TYPE::CUBE:
		{
			break;
		}
		case eDEBUGSHAPE_TYPE::SPHERE:
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
	pCam->SetCamIndex(eCAMERA_INDEX::EDITOR);

	CTransform* pTransform = new CTransform;
	m_pEditorCam->AddComponent(pTransform);
	pTransform->SetRelativePos(Vec3(0.f, 0.f, -100.f));

	m_pEditorCam->AddScript(new CScript_CameraMove);

	CRenderMgr::GetInst()->SetEditorCam(m_pEditorCam->Camera());
}

void CEditorObjMgr::MouseLBTNCallback(CGameObject* _pObj)
{
	static CUI_Inspector* pInspector = static_cast<CUI_Inspector*>(CImGuiMgr::GetInst()->FindUI("Inspector"));

	if (nullptr != pInspector)
		pInspector->SetTarget(_pObj);
}
