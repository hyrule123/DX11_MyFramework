#include "pch.h"
#include "cUIobj_Inspector.h"

#include "macroFunc_Imgui.h"

//CComponents
#include "cUIobj_Collider2D.h"
#include "cUIobj_Animator2D.h"
#include "cUIobj_Light2D.h"
#include "cUIobj_Tilemap.h"
#include "cUIobj_Camera.h"
#include "cUIobj_Renderer_Basic.h"
#include "cUIobj_ScriptHolder.h"
#include "cUIobj_ParticleSystem.h"

//Search Target
#include <Engine/cGameObject.h>
#include <Engine/cLevelMgr.h>
#include <Engine/CKeyMgr.h>
#include <Engine/cCom_Camera.h>
#include <Engine/cTransform.h>
#include <Engine/cRenderMgr.h>

#include <Engine/jsoncpp.h>

//Event Dispatcher
#include <Engine/EventDispatcher.h>

cUIobj_Inspector::cUIobj_Inspector()
	: cUI_BasicWindow("Inspector")
	, m_pTarget(nullptr)
	, m_arrComUI{}
{
	SetSaveEnable(true);
	
	m_arrComUI[(UINT)eCOMPONENT_TYPE::RENDERER] = new cUIobj_Renderer_Basic;
	m_arrComUI[(UINT)eCOMPONENT_TYPE::RENDERER]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[(UINT)eCOMPONENT_TYPE::RENDERER]);

	//m_arrComUI[(UINT)eCOMPONENT_TYPE::] = new cUIobj_ParticleSystem;
	//m_arrComUI[(UINT)eCOMPONENT_TYPE::PARTICLE_SYSTEM]->SetSize(0.f, 150.f);
	//AddChildUI(m_arrComUI[(UINT)eCOMPONENT_TYPE::PARTICLE_SYSTEM]);

	m_arrComUI[(UINT)eCOMPONENT_TYPE::CAMERA] = new cUIobj_Camera;
	m_arrComUI[(UINT)eCOMPONENT_TYPE::CAMERA]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[(UINT)eCOMPONENT_TYPE::CAMERA]);

	m_arrComUI[(UINT)eCOMPONENT_TYPE::COLLIDER] = new cUIobj_Collider2D;
	m_arrComUI[(UINT)eCOMPONENT_TYPE::COLLIDER]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[(UINT)eCOMPONENT_TYPE::COLLIDER]);

	m_arrComUI[(UINT)eCOMPONENT_TYPE::ANIMATOR] = new cUIobj_Animator2D;
	m_arrComUI[(UINT)eCOMPONENT_TYPE::ANIMATOR]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[(UINT)eCOMPONENT_TYPE::ANIMATOR]);

	m_arrComUI[(UINT)eCOMPONENT_TYPE::LIGHT] = new cUIobj_Light2D;
	m_arrComUI[(UINT)eCOMPONENT_TYPE::LIGHT]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[(UINT)eCOMPONENT_TYPE::LIGHT]);

	//m_arrComUI[(UINT)eCOMPONENT_TYPE::] = new cUIobj_Tilemap;
	//m_arrComUI[(UINT)eCOMPONENT_TYPE::TILEMAP]->SetSize(0.f, 150.f);
	//AddChildUI(m_arrComUI[(UINT)eCOMPONENT_TYPE::TILEMAP]);
}

cUIobj_Inspector::~cUIobj_Inspector()
{
	//클래스 인스턴스 삭제는 Child UI를 순회 돌아주면서 삭제되므로 여기서 할 필요 없음.
}

void cUIobj_Inspector::init()
{
}

void cUIobj_Inspector::Tick()
{
	//매 Tick마다 컴포넌트의 유뮤를 확인하여 갱신
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
}

void cUIobj_Inspector::render_update()
{
	if (m_pTarget)
	{
		int Layer = m_pTarget->GetLayer();
		//IMGUI에 받아온 값을 표시
		ImGui::Text("Layer Index");
		ImGui::SameLine(100.f);
		bool Changed = ImGui::DragInt("##Layer", &Layer);

		TransformIndicator();

	}

}


void cUIobj_Inspector::SetTarget(cGameObject* _pTarget)
{
	m_pTarget = _pTarget;
	for (int i = 0; i < (int)eCOMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrComUI[i])
			m_arrComUI[i]->SetTarget(_pTarget);
	}
}

void cUIobj_Inspector::TransformIndicator()
{
	//타겟 오브젝트가 있을 경우에는 컴포넌트 구분을 표시
	IMGUI_Indicator(GetKey().c_str(), ImColorPreset::DarkRed);
	cTransform& pTransform = m_pTarget->Transform();

	//값을 수정도 가능해야 하므로 일반 value로 받아온다.
	Vec3 vPos = pTransform.GetRelativePos();
	Vec3 vSize = pTransform.GetSize();
	Vec3 vScale = pTransform.GetRelativeScale();
	Vec3 vRot = (pTransform.GetRelativeRot() / XM_PI) * 180.f;


	//IMGUI에 받아온 값을 표시
	ImGui::Text("Position");
	ImGui::SameLine(100.f);
	bool PosChanged = ImGui::DragFloat3("##Relative Position", vPos);


	ImGui::Text("Size");
	ImGui::SameLine(100.f);
	bool SizeChanged = ImGui::DragFloat3("##Size", vSize, 1.f, 0.f, FLT_MAX);


	ImGui::Text("Scale");
	ImGui::SameLine(100.f);
	bool ScaleChanged = ImGui::DragFloat3("##Relative Scale", vScale, 0.1f, 0.f, FLT_MAX);


	ImGui::Text("Rotation");
	ImGui::SameLine(100.f);
	bool RotationChanged = ImGui::DragFloat3("##Relative Rotation", vRot);



	//값이 변경되었을 경우 다시 값을 설정.
	if (true == PosChanged)
		pTransform.SetRelativePos(vPos);

	else if (true == SizeChanged)
		pTransform.SetSize(vSize);

	else if (true == ScaleChanged)
		pTransform.SetRelativeScale(vScale);

	else if (true == RotationChanged)
	{
		vRot = (vRot / 180.f) * XM_PI;
		pTransform.SetRelativeRot(vRot);
	}
}

void cUIobj_Inspector::Save(Json::Value& _Save)
{
	_Save[GetStrID()]["IsActive"] = GetActive();
}

void cUIobj_Inspector::Load(Json::Value& _Load)
{
	if (_Load[GetStrID()])
	{
		SetActive(_Load[GetStrID()]["IsActive"].as<bool>());
	}
}

