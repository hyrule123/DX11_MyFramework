#include "pch.h"
#include "CUIobj_Inspector.h"

//CComponents
#include "CUIobj_Transform.h"
#include "CUIobj_Collider2D.h"
#include "CUIobj_Animator2D.h"
#include "CUIobj_Light2D.h"
#include "CUIobj_Tilemap.h"
#include "CUIobj_Camera.h"
#include "CUIobj_MeshRender.h"
#include "CUIobj_ScriptHolder.h"
#include "CUIobj_ParticleSystem.h"

//Search Target
#include <Engine/CGameObject.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CCamera.h>
#include <Engine/CTransform.h>
#include <Engine/CRenderMgr.h>

#include "JsonCPP.h"

CUIobj_Inspector::CUIobj_Inspector()
	: CUI_BasicWindow("Inspector")
	, m_pTarget(nullptr)
	, m_arrComUI{}
{
	SetSaveEnable(true);


	m_arrComUI[(UINT)eCOMPONENT_TYPE::TRANSFORM] = new CUIobj_Transform;
	m_arrComUI[(UINT)eCOMPONENT_TYPE::TRANSFORM]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[(UINT)eCOMPONENT_TYPE::TRANSFORM]);
	
	m_arrComUI[(UINT)eCOMPONENT_TYPE::MESH_RENDER] = new CUIobj_MeshRender;
	m_arrComUI[(UINT)eCOMPONENT_TYPE::MESH_RENDER]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[(UINT)eCOMPONENT_TYPE::MESH_RENDER]);

	m_arrComUI[(UINT)eCOMPONENT_TYPE::PARTICLE_SYSTEM] = new CUIobj_ParticleSystem;
	m_arrComUI[(UINT)eCOMPONENT_TYPE::PARTICLE_SYSTEM]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[(UINT)eCOMPONENT_TYPE::PARTICLE_SYSTEM]);

	m_arrComUI[(UINT)eCOMPONENT_TYPE::CAMERA] = new CUIobj_Camera;
	m_arrComUI[(UINT)eCOMPONENT_TYPE::CAMERA]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[(UINT)eCOMPONENT_TYPE::CAMERA]);

	m_arrComUI[(UINT)eCOMPONENT_TYPE::COLLIDER2D] = new CUIobj_Collider2D;
	m_arrComUI[(UINT)eCOMPONENT_TYPE::COLLIDER2D]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[(UINT)eCOMPONENT_TYPE::COLLIDER2D]);

	m_arrComUI[(UINT)eCOMPONENT_TYPE::ANIMATOR2D] = new CUIobj_Animator2D;
	m_arrComUI[(UINT)eCOMPONENT_TYPE::ANIMATOR2D]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[(UINT)eCOMPONENT_TYPE::ANIMATOR2D]);

	m_arrComUI[(UINT)eCOMPONENT_TYPE::LIGHT2D] = new CUIobj_Light2D;
	m_arrComUI[(UINT)eCOMPONENT_TYPE::LIGHT2D]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[(UINT)eCOMPONENT_TYPE::LIGHT2D]);

	m_arrComUI[(UINT)eCOMPONENT_TYPE::TILEMAP] = new CUIobj_Tilemap;
	m_arrComUI[(UINT)eCOMPONENT_TYPE::TILEMAP]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[(UINT)eCOMPONENT_TYPE::TILEMAP]);
}

CUIobj_Inspector::~CUIobj_Inspector()
{
	//클래스 인스턴스 삭제는 Child UI를 순회 돌아주면서 삭제되므로 여기서 할 필요 없음.
}

void CUIobj_Inspector::init()
{
}

void CUIobj_Inspector::tick()
{
	////매 tick마다 컴포넌트의 유뮤를 확인하여 갱신
	//if (nullptr != m_pTarget)
	//{
	//	for (int i = 0; i < (int)eCOMPONENT_TYPE::END; ++i)
	//	{
	//		bool b = nullptr != m_pTarget->GetComponent((eCOMPONENT_TYPE)i);
	//		
	//		if (nullptr != m_arrComUI[i])
	//		{
	//			m_arrComUI[i]->SetActive(b);
	//		}
	//	}
	//}
}


void CUIobj_Inspector::SetTarget(CGameObject* _pTarget)
{
	m_pTarget = _pTarget;
	for (int i = 0; i < (int)eCOMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrComUI[i])
			m_arrComUI[i]->SetTarget(_pTarget);
	}
}

void CUIobj_Inspector::Save(Json::Value& _Save)
{
	_Save[GetStrID()]["IsActive"] = GetActive();
}

void CUIobj_Inspector::Load(Json::Value& _Load)
{
	if (_Load[GetStrID()])
	{
		SetActive(_Load[GetStrID()]["IsActive"].as<bool>());
	}
}

