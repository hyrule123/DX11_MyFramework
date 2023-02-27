#include "pch.h"
#include "CUI_Inspector.h"

//CComponents
#include "CUI_Transform.h"
#include "CUI_Collider2D.h"
#include "CUI_Animator2D.h"
#include "CUI_Light2D.h"
#include "CUI_Tilemap.h"
#include "CUI_Camera.h"
#include "CUI_MeshRender.h"
#include "CUI_ScriptHolder.h"
#include "CUI_ParticleSystem.h"

//Search Target
#include <Engine/CGameObject.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CCamera.h>
#include <Engine/CTransform.h>
#include <Engine/CRenderMgr.h>

#include "JsonCPP.h"

CUI_Inspector::CUI_Inspector()
	: CUI_BasicWindow("Inspector")
	, m_pTarget(nullptr)
	, m_arrComUI{}
{
	


	m_arrComUI[(UINT)eCOMPONENT_TYPE::TRANSFORM] = new CUI_Transform;
	m_arrComUI[(UINT)eCOMPONENT_TYPE::TRANSFORM]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[(UINT)eCOMPONENT_TYPE::TRANSFORM]);
	
	m_arrComUI[(UINT)eCOMPONENT_TYPE::MESH_RENDER] = new CUI_MeshRender;
	m_arrComUI[(UINT)eCOMPONENT_TYPE::MESH_RENDER]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[(UINT)eCOMPONENT_TYPE::MESH_RENDER]);

	m_arrComUI[(UINT)eCOMPONENT_TYPE::PARTICLE_SYSTEM] = new CUI_ParticleSystem;
	m_arrComUI[(UINT)eCOMPONENT_TYPE::PARTICLE_SYSTEM]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[(UINT)eCOMPONENT_TYPE::PARTICLE_SYSTEM]);

	m_arrComUI[(UINT)eCOMPONENT_TYPE::CAMERA] = new CUI_Camera;
	m_arrComUI[(UINT)eCOMPONENT_TYPE::CAMERA]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[(UINT)eCOMPONENT_TYPE::CAMERA]);

	m_arrComUI[(UINT)eCOMPONENT_TYPE::COLLIDER2D] = new CUI_Collider2D;
	m_arrComUI[(UINT)eCOMPONENT_TYPE::COLLIDER2D]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[(UINT)eCOMPONENT_TYPE::COLLIDER2D]);

	m_arrComUI[(UINT)eCOMPONENT_TYPE::ANIMATOR2D] = new CUI_Animator2D;
	m_arrComUI[(UINT)eCOMPONENT_TYPE::ANIMATOR2D]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[(UINT)eCOMPONENT_TYPE::ANIMATOR2D]);

	m_arrComUI[(UINT)eCOMPONENT_TYPE::LIGHT2D] = new CUI_Light2D;
	m_arrComUI[(UINT)eCOMPONENT_TYPE::LIGHT2D]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[(UINT)eCOMPONENT_TYPE::LIGHT2D]);

	m_arrComUI[(UINT)eCOMPONENT_TYPE::TILEMAP] = new CUI_Tilemap;
	m_arrComUI[(UINT)eCOMPONENT_TYPE::TILEMAP]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[(UINT)eCOMPONENT_TYPE::TILEMAP]);
}

CUI_Inspector::~CUI_Inspector()
{
	//클래스 인스턴스 삭제는 Child UI를 순회 돌아주면서 삭제되므로 여기서 할 필요 없음.
}

void CUI_Inspector::init()
{
	m_pTarget = CLevelMgr::GetInst()->FindObjectByName("Player");

	for (int i = 0; i < (int)eCOMPONENT_TYPE::END; ++i)
	{
		if (nullptr == m_arrComUI[i])
			continue;

		m_arrComUI[i]->SetTarget(m_pTarget);
	}
}

void CUI_Inspector::tick()
{

	if (nullptr == m_pTarget)
		init();

	//매 tick마다 컴포넌트의 유뮤를 확인하여 갱신
	if (nullptr != m_pTarget)
	{
		for (int i = 0; i < (int)eCOMPONENT_TYPE::END; ++i)
		{
			bool b = nullptr != m_pTarget->GetComponent((eCOMPONENT_TYPE)i);
			
			if (nullptr != m_arrComUI[i])
			{
				m_arrComUI[i]->SetActive(b);
			}
		}
	}


	////여기서는 나중에 마우스 클릭을 받아서 클릭된 위치에 있는 오브젝트의 주소를 가져오는 코드를 구현
	//CKeyMgr* KeyMgr = CKeyMgr::GetInst();
	//if (eKEY_STATE::TAP == KeyMgr->GetKeyState(KEY::LBTN))
	//{
	//	//마우스 위치를 받아온다. 이 좌표는 NDC 좌표계와 반대이므로 y축은 반전을 해준다.
	//	Vec3 vMouseWorldPos(KeyMgr->GetMousePos(), 0.f);
	//	vMouseWorldPos.y = g_GlobalVal.vResolution.y - vMouseWorldPos.y;

	//	//NDC 좌표계는 원점이 중점이다. -> 해상도의 반만큼 빼준다.
	//	vMouseWorldPos.x -= g_GlobalVal.vResolution.x * 0.5f;
	//	vMouseWorldPos.y -= g_GlobalVal.vResolution.y * 0.5f;

	//	CCamera* pCam = CRenderMgr::GetInst()->GetCurCamera();
	//	const Vec3& CamPos = pCam->Transform()->GetWorldPos();

	//	//구한 좌표에 카메라의 위치를 더한다. -> 월드상에서의 좌표로 변환 완료.
	//	vMouseWorldPos += CamPos;

	//	//해당 방향에서 카메라가 보는 방향으로 가상의 직선을 만들고 충돌검사를 실시한다.

	//	

	//	//마우스 위치를 
	//	

	//	//마우스 위치를 더한다.
	//	//vMouseWorldPos.x += MousePos.x;
	//	//vMouseWorldPos.y -= MousePos.y;
	//	
	//	//마우스 좌표를 NDC 좌표 형식으로 변환한다.(해상도 반 곱한걸 빼줌)





 //
	//	

	//	//Vec4 vMousePos = Vec4(MousePos.x, MousePos.y, 0.f, 1.f);


	//}

}


void CUI_Inspector::SetTarget(CGameObject* _pTarget)
{
	m_pTarget = _pTarget;
	for (int i = 0; i < (int)eCOMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrComUI[i])
			m_arrComUI[i]->SetTarget(_pTarget);
	}
}

void CUI_Inspector::Save(Json::Value& _Save)
{
	_Save[GetStrID()]["IsActive"] = GetActive();
}

void CUI_Inspector::Load(Json::Value& _Load)
{
	if (_Load[GetStrID()])
	{
		SetActive(_Load[GetStrID()]["IsActive"].as<bool>());
	}
}

