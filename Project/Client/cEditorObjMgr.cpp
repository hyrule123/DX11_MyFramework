#include "pch.h"

#include "cEditorObjMgr.h"
#include "cImGuiMgr.h"
#include "cUIobj_Inspector.h"

#include <Engine/cGameObject.h>
#include <Engine/CTimeMgr.h>
#include <Engine/cRenderMgr.h>
#include <Engine/cResMgr.h>
#include <Engine/cCom_Renderer_Default.h>
#include <Engine/cCom_Camera.h>
#include <Engine/cTransform.h>
#include <Engine/strKey_Default.h>

#include <Engine/cCollisionMgr.h>
#include <Engine/cCom_Coll2D_Point.h>

#include <Engine/S_H_Debug.hlsli>

#include <Script/CScript_CameraMove.h>
#include <Script/CScript_MouseCursor.h>

#include <Script/strKey_Component.h>

#include <Engine/EventDispatcher.h>

//테스트용 레벨
#include "MainGameLevel.h"

#include <Script/defineScript.h>

#include <Script/define_SC.h>

#include <Engine/cComMgr.h>

#include <Script/cScript_CameraMove.h>

cEditorObjMgr::cEditorObjMgr()
	: m_arrDebugShape{}
	, m_pEditorCam()
	, m_pMousePicker()
{
}
cEditorObjMgr::~cEditorObjMgr()
{
	for (int i = 0; i < (int)eDEBUGSHAPE_TYPE::END; ++i)
	{
		SAFE_DELETE(m_arrDebugShape[i]);
	}

	SAFE_DELETE(m_pEditorCam);
	//DESTRUCTOR_DELETE(m_pMousePicker);
}


void cEditorObjMgr::init()
{
	::CreateMainGame();

	CreateDebugShape();

	CreateEditorCamera();

	{//커서 생성
		m_pMousePicker = new cGameObject;
		m_pMousePicker->SetKey("Cursor");
		//m_pMousePicker->AddComponent(new cTransform);
		m_pMousePicker->AddComponent<cCom_Coll2D_Point>();



		cScript_MouseCursor* pScript = m_pMousePicker->AddComponent< cScript_MouseCursor>();
		pScript->AddFuncLBTNCallback(eKEY_STATE::DOWN, std::bind(&cEditorObjMgr::MouseLBTNCallback, this, std::placeholders::_1));

 		EventDispatcher::SpawnGameObject(m_pMousePicker, Vec3(0.f, 0.f, 0.f), SC::LAYER_INFO::MouseCursor);

		cCollisionMgr::GetInst()->AddLayerInterAction2DAll(SC::LAYER_INFO::MouseCursor);
	}
}

void cEditorObjMgr::progress()
{
	Tick();
	render();
}

//Engine으로부터 디버그에 사용할 오브젝트들 정보를 받아오기
void cEditorObjMgr::Tick()
{
	//이 메소드의 호출 시점은 모든 게임의 메인 렌더링과정이 종료된 이후이다. 
	


	//cRenderMgr로부터 이번 프레임에 그릴 디버그 쉐이프 목록을 받아온다.
	cRenderMgr::GetInst()->UpdateDebugShapeRender(m_vecDebugShapeInfo);

	float DT = DELTA_TIME;

	//여기선 단순히 시간만 뺴 준다. render()에서 그리고 나서 음수이면 제거함.
	size_t size = m_vecDebugShapeInfo.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecDebugShapeInfo[i].fLifeSpan -= DT;
	}


	m_pEditorCam->Tick();
	m_pEditorCam->FinalTick();
}

//받아온 오브젝트를 통해서 디버그 정보를 사각형으로 그리기
void cEditorObjMgr::render()
{
	cCom_Camera* pCam = cRenderMgr::GetInst()->GetCurCamera();
	//카메라의 데이터를 업로드
	

	size_t size = m_vecDebugShapeInfo.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecDebugShapeInfo[i].fLifeSpan -= DELTA_TIME;


		//월드행렬 전달.
		tMtrlScalarData MtrlData = {};
		Ptr<cMaterial> pMtrl = nullptr;
		tInstancingKey Key = {};

		//여기서 설정해줘야 하는것들
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
			Key.pMtrl = (DWORD_PTR)m_arrDebugShape[(int)eDEBUGSHAPE_TYPE::RECT]->Renderer()->GetCurMaterial().Get();
			Key.pMesh = (DWORD_PTR)m_arrDebugShape[(int)eDEBUGSHAPE_TYPE::RECT]->Renderer()->GetMesh().Get();

			MtrlData.INT_0 = (int)pCam->GetCamIndex();

			MtrlData.INT_1 = (int)eDEBUGSHAPE_TYPE::RECT;

			//MAT_WORLD = 1번, VP 행렬은 g_CBuffer_matCam에 있으므로 따로 전달하지 않음
			MtrlData.MAT_1 = m_vecDebugShapeInfo[i].matWorld;

			

			//m_arrDebugShape[(int)eDEBUGSHAPE_TYPE::RECT]->SetMtrlScalarParam(MTRL_SCALAR_DEBUG_MAT_WORLD, m_vecDebugShapeInfo[i].matWorld.m);

			//m_arrDebugShape[(int)eDEBUGSHAPE_TYPE::RECT]->SetMtrlScalarParam(MTRL_SCALAR_DEBUG_VEC4_COLOR, &(m_vecDebugShapeInfo[i].vColor));

			//레이어에 속해서 게임 내에서 돌아가는 게임오브젝트가 아니므로 강제로 render()를 호출해야 한다.
			/*m_arrDebugShape[(int)eDEBUGSHAPE_TYPE::RECT]->render();*/

			break;
		}
		case eDEBUGSHAPE_TYPE::CIRCLE:
		{
			Key.pMtrl = (DWORD_PTR)m_arrDebugShape[(int)eDEBUGSHAPE_TYPE::CIRCLE]->Renderer()->GetCurMaterial().Get();
			Key.pMesh = (DWORD_PTR)m_arrDebugShape[(int)eDEBUGSHAPE_TYPE::CIRCLE]->Renderer()->GetMesh().Get();

			MtrlData.INT_1 = (int)eDEBUGSHAPE_TYPE::CIRCLE;

			//CIRCLE의 경우 정점의 갯수가 많으므로 월드행렬 대신 WVP 행렬을 만들어서 전달
			//MAT_WVP = 0번
			MtrlData.MAT_0 = m_vecDebugShapeInfo[i].matWorld * g_matCam[(int)pCam->GetCamIndex()].matVP;

			//월드행렬 전달.
			//const Matrix& matWorld = m_vecDebugShapeInfo[i].matWorld;

			//m_arrDebugShape[(int)eDEBUGSHAPE_TYPE::CIRCLE]->SetMtrlScalarParam(MTRL_SCALAR_DEBUG_MAT_WORLD, m_vecDebugShapeInfo[i].matWorld.m);

			//m_arrDebugShape[(int)eDEBUGSHAPE_TYPE::CIRCLE]->SetMtrlScalarParam(MTRL_SCALAR_DEBUG_VEC4_COLOR, &(m_vecDebugShapeInfo[i].vColor));

			////레이어에 속해서 게임 내에서 돌아가는 게임오브젝트가 아니므로 강제로 render()를 호출해야 한다.
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


		//MAT_COLOR = 0번
		MtrlData.VEC4_0 = m_vecDebugShapeInfo[i].vColor;


		//재질 정보를 재질에 등록한다.
		cRenderMgr::GetInst()->AddInstancingQueue(Key, MtrlData);
	}

	cRenderMgr::GetInst()->InstancedRender();
	//for (int i = 0; i < (int)eDEBUGSHAPE_TYPE::END; ++i)
	//{
	//	CRenderComponent* pRenderCom = m_arrDebugShape[i]->GetRenderComponent();
	//	if (nullptr == pRenderCom)
	//		continue;

	//	cMaterial* pMtrl = pRenderCom->GetCurMaterial().Get();
	//	pMtrl->BindData();
	//	pRenderCom->GetMesh()->renderInstanced(pMtrl->GetInstancingCount());
	//}


	//남은 시간이 음수가 된 디버그 쉐이프에 대해 삭제 처리를 진행
	m_vecDebugShapeInfo.erase(
		std::remove_if(m_vecDebugShapeInfo.begin(), m_vecDebugShapeInfo.end(),
			[](const tDebugShapeInfo& _Info)->bool
			{
				return (_Info.fLifeSpan < 0.f);
			})
		, m_vecDebugShapeInfo.end());
}

void cEditorObjMgr::CreateDebugShape()
{
	for (int i = 0; i < (int)eDEBUGSHAPE_TYPE::END; ++i)
	{
		m_arrDebugShape[i] = new cGameObject;

		switch ((eDEBUGSHAPE_TYPE)i)
		{
		case eDEBUGSHAPE_TYPE::RECT:
		{
			cCom_Renderer_Default* pMesh = new cCom_Renderer_Default;
			//월드행렬을 직접 받아서 쉐이더에 보낼 것이기 떄문에 Transform은 필요하지 않음.
			Ptr<cMesh> pDebugMesh = cResMgr::GetInst()->FindRes<cMesh>(strKey_RES_DEFAULT::MESH::DEBUG_RECT);
			Ptr<cMaterial> pDebugMtrl = cResMgr::GetInst()->FindRes<cMaterial>(strKey_RES_DEFAULT::MATERIAL::DEBUG_RECT);
			pMesh->SetMesh(pDebugMesh);
			pMesh->SetMaterial(pDebugMtrl);
			m_arrDebugShape[i]->AddComponent(pMesh);

			break;
		}

		case eDEBUGSHAPE_TYPE::CIRCLE:
		{
			cCom_Renderer_Default* pMesh = new cCom_Renderer_Default;
			Ptr<cMesh> pDebugMesh = cResMgr::GetInst()->FindRes<cMesh>(strKey_RES_DEFAULT::MESH::DEBUG_CIRCLE);
			Ptr<cMaterial> pDebugMtrl = cResMgr::GetInst()->FindRes<cMaterial>(strKey_RES_DEFAULT::MATERIAL::DEBUG_CIRCLE);
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

void cEditorObjMgr::CreateEditorCamera()
{
	m_pEditorCam = new cGameObject;

	cCom_Camera* pCam = new cCom_Camera;
	m_pEditorCam->AddComponent(pCam);
	pCam->SetLayerFlag(UINT32_MAX);
	pCam->SetCamIndex(eCAMERA_INDEX::EDITOR);

	cTransform& pTransform = m_pEditorCam->Transform();
	pTransform.SetRelativePos(Vec3(0.f, 0.f, -100.f));
m_pEditorCam->AddComponent<cScript_CameraMove>();

	cRenderMgr::GetInst()->SetEditorCam(m_pEditorCam->Camera());
}

cGameObject* cEditorObjMgr::GetSelectedObj()
{
	if (nullptr == m_pMousePicker || nullptr == m_MouseScript)
		return nullptr;



	return m_MouseScript->GetSelectedObj();
}

void cEditorObjMgr::MouseLBTNCallback(cGameObject* _pObj)
{
	static cUIobj_Inspector* pInspector = static_cast<cUIobj_Inspector*>(cImGuiMgr::GetInst()->FindUI("Inspector"));

	if (nullptr != pInspector)
		pInspector->SetTarget(_pObj);
}
