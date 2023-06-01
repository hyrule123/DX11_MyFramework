#include "pch.h"

#include "CEditorObjMgr.h"
#include "CImGuiMgr.h"
#include "CUIobj_Inspector.h"

#include <Engine/CGameObject.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CResMgr.h>
#include <Engine/CMeshRender.h>
#include <Engine/CCamera.h>
#include <Engine/CTransform.h>
#include <Engine/strKey_Default.h>

#include <Engine/CCollisionMgr.h>
#include <Engine/CCollider2D_Point.h>

#include <Engine/S_0_H_Debug.hlsli>

#include <Engine/CScriptHolder.h>
#include <Script/CScript_CameraMove.h>
#include <Script/CScript_MouseCursor.h>

#include <Script/strKey_Script.h>
#include <Engine/CScriptMgr.h>

#include <Engine/EventDispatcher.h>

//테스트용 레벨
#include "CMainGameLevel.h"

#include <Script/defineScript.h>


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
		SAFE_DELETE(m_arrDebugShape[i]);
	}

	SAFE_DELETE(m_pEditorCam);
	//DESTRUCTOR_DELETE(m_pMousePicker);
}


void CEditorObjMgr::init()
{
	::CreateMainGame();

	CreateDebugShape();

	CreateEditorCamera();

	{//커서 생성
		m_pMousePicker = new CGameObject;
		m_pMousePicker->SetName("Cursor");
		//m_pMousePicker->AddComponent(new CTransform);
		m_pMousePicker->AddComponent(new CCollider2D_Point);

		CScript* Script = CScriptMgr::GetInst()->GetNewScript(strKey_SCRIPTS::MOUSECURSOR);
		m_pMousePicker->AddScript(Script);

		CScript_MouseCursor* pScript = static_cast<CScript_MouseCursor*>(m_pMousePicker->ScriptHolder()->FindScript(strKey_SCRIPTS::MOUSECURSOR));
		pScript->AddFuncLBTNCallback(eKEY_STATE::TAP, std::bind(&CEditorObjMgr::MouseLBTNCallback, this, std::placeholders::_1));

 		EventDispatcher::SpawnGameObject(m_pMousePicker, Vec3(0.f, 0.f, 0.f), iLayerCursor);

		CCollisionMgr* pMgr = CCollisionMgr::GetInst();
		for (int i = 0; i < 32; ++i)
		{
			pMgr->AddLayerInteraction2D(iLayerCursor, i);
		}
	}
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
	CCamera* pCam = CRenderMgr::GetInst()->GetCurCamera();
	//카메라의 데이터를 업로드
	

	size_t size = m_vecDebugShapeInfo.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecDebugShapeInfo[i].fLifeSpan -= DELTA_TIME;


		//월드행렬 전달.
		tMtrlScalarData MtrlData = {};
		Ptr<CMaterial> pMtrl = nullptr;
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
			Key.pMtrl = (DWORD_PTR)m_arrDebugShape[(int)eDEBUGSHAPE_TYPE::RECT]->MeshRender()->GetCurMaterial().Get();
			Key.pMesh = (DWORD_PTR)m_arrDebugShape[(int)eDEBUGSHAPE_TYPE::RECT]->MeshRender()->GetMesh().Get();

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
			Key.pMtrl = (DWORD_PTR)m_arrDebugShape[(int)eDEBUGSHAPE_TYPE::CIRCLE]->MeshRender()->GetCurMaterial().Get();
			Key.pMesh = (DWORD_PTR)m_arrDebugShape[(int)eDEBUGSHAPE_TYPE::CIRCLE]->MeshRender()->GetMesh().Get();

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
		CRenderMgr::GetInst()->AddInstancingQueue(Key, MtrlData);
	}

	CRenderMgr::GetInst()->InstancedRender();
	//for (int i = 0; i < (int)eDEBUGSHAPE_TYPE::END; ++i)
	//{
	//	CRenderComponent* pRenderCom = m_arrDebugShape[i]->GetRenderComponent();
	//	if (nullptr == pRenderCom)
	//		continue;

	//	CMaterial* pMtrl = pRenderCom->GetCurMaterial().Get();
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
			//월드행렬을 직접 받아서 쉐이더에 보낼 것이기 떄문에 Transform은 필요하지 않음.
			Ptr<CMesh> pDebugMesh = CResMgr::GetInst()->FindRes<CMesh>(strKey_RES_DEFAULT::MESH::DEBUG_RECT);
			Ptr<CMaterial> pDebugMtrl = CResMgr::GetInst()->FindRes<CMaterial>(strKey_RES_DEFAULT::MATERIAL::DEBUG_RECT);
			pMesh->SetMesh(pDebugMesh);
			pMesh->SetMaterial(pDebugMtrl);
			m_arrDebugShape[i]->AddComponent(pMesh);

			break;
		}

		case eDEBUGSHAPE_TYPE::CIRCLE:
		{
			CMeshRender* pMesh = new CMeshRender;
			Ptr<CMesh> pDebugMesh = CResMgr::GetInst()->FindRes<CMesh>(strKey_RES_DEFAULT::MESH::DEBUG_CIRCLE);
			Ptr<CMaterial> pDebugMtrl = CResMgr::GetInst()->FindRes<CMaterial>(strKey_RES_DEFAULT::MATERIAL::DEBUG_CIRCLE);
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

	CTransform* pTransform = m_pEditorCam->Transform();
	pTransform->SetRelativePos(Vec3(0.f, 0.f, -100.f));

	m_pEditorCam->AddScript(CScriptMgr::GetInst()->GetNewScript(strKey_SCRIPTS::CAMERAMOVE));

	CRenderMgr::GetInst()->SetEditorCam(m_pEditorCam->Camera());
}

CGameObject* CEditorObjMgr::GetSelectedObj()
{
	if (nullptr == m_pMousePicker)
		return nullptr;

	return static_cast<CScript_MouseCursor*>(m_pMousePicker->ScriptHolder()->FindScript(strKey_SCRIPTS::MOUSECURSOR))->GetSelectedObject();
}

void CEditorObjMgr::MouseLBTNCallback(CGameObject* _pObj)
{
	static CUIobj_Inspector* pInspector = static_cast<CUIobj_Inspector*>(CImGuiMgr::GetInst()->FindUI("Inspector"));

	if (nullptr != pInspector)
		pInspector->SetTarget(_pObj);
}
