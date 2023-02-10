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

//Search Target
#include <Engine/CGameObject.h>
#include <Engine/CLevelMgr.h>

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
	//여기서는 나중에 마우스 클릭을 받아서 클릭된 위치에 있는 오브젝트의 주소를 가져오는 코드를 구현
}

void CUI_Inspector::Save(Json::Value& _Save)
{
	_Save[GetName()]["IsActive"] = GetActive();
}

void CUI_Inspector::Load(Json::Value& _Load)
{
	if (_Load[GetName()])
	{
		SetActive(_Load[GetName()]["IsActive"].as<bool>());
	}
}

