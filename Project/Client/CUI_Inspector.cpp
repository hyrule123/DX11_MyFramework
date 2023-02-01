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

CUI_Inspector::CUI_Inspector()
	: CUI_BasicWindow("Inspector")
	, m_pTarget(nullptr)
	, m_arrComUI{}
{

	m_arrComUI[eCOMPONENT_TRANSFORM] = new CUI_Transform;
	m_arrComUI[eCOMPONENT_TRANSFORM]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[eCOMPONENT_TRANSFORM]);
	
	m_arrComUI[eCOMPONENT_MESH_RENDER] = new CUI_MeshRender;
	m_arrComUI[eCOMPONENT_MESH_RENDER]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[eCOMPONENT_MESH_RENDER]);

	m_arrComUI[eCOMPONENT_CAMERA] = new CUI_Camera;
	m_arrComUI[eCOMPONENT_CAMERA]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[eCOMPONENT_CAMERA]);

	m_arrComUI[eCOMPONENT_COLLIDER2D] = new CUI_Collider2D;
	m_arrComUI[eCOMPONENT_COLLIDER2D]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[eCOMPONENT_COLLIDER2D]);

	m_arrComUI[eCOMPONENT_ANIMATOR2D] = new CUI_Animator2D;
	m_arrComUI[eCOMPONENT_ANIMATOR2D]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[eCOMPONENT_ANIMATOR2D]);

	m_arrComUI[eCOMPONENT_LIGHT2D] = new CUI_Light2D;
	m_arrComUI[eCOMPONENT_LIGHT2D]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[eCOMPONENT_LIGHT2D]);

	m_arrComUI[eCOMPONENT_TILEMAP] = new CUI_Tilemap;
	m_arrComUI[eCOMPONENT_TILEMAP]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[eCOMPONENT_TILEMAP]);
}

CUI_Inspector::~CUI_Inspector()
{
	//Ŭ���� �ν��Ͻ� ������ Child UI�� ��ȸ �����ָ鼭 �����ǹǷ� ���⼭ �� �ʿ� ����.
}

void CUI_Inspector::init()
{
	m_pTarget = CLevelMgr::GetInst()->FindObjectByName("Player");

	for (int i = 0; i < eCOMPONENT_END; ++i)
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
	//���⼭�� ���߿� ���콺 Ŭ���� �޾Ƽ� Ŭ���� ��ġ�� �ִ� ������Ʈ�� �ּҸ� �������� �ڵ带 ����
}

