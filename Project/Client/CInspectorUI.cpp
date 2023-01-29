#include "pch.h"
#include "CInspectorUI.h"

//CComponents
#include "CTransformUI.h"
#include "CCollider2DUI.h"
#include "CAnimator2DUI.h"
#include "CLight2DUI.h"
#include "CTilemapUI.h"
#include "CCameraUI.h"
#include "CMeshRenderUI.h"
#include "CScriptHolderUI.h"

//Search Target
#include <Engine/CGameObject.h>
#include <Engine/CLevelMgr.h>

CInspectorUI::CInspectorUI()
	: CUI("##Inspector")
	, m_pTarget(nullptr)
	, m_arrComUI{}
{
	SetName("Inspector");

	m_arrComUI[eCOMPONENT_TRANSFORM] = new CTransformUI;
	m_arrComUI[eCOMPONENT_TRANSFORM]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[eCOMPONENT_TRANSFORM]);
	
	m_arrComUI[eCOMPONENT_MESH_RENDER] = new CMeshRenderUI;
	m_arrComUI[eCOMPONENT_MESH_RENDER]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[eCOMPONENT_MESH_RENDER]);

	m_arrComUI[eCOMPONENT_CAMERA] = new CCameraUI;
	m_arrComUI[eCOMPONENT_CAMERA]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[eCOMPONENT_CAMERA]);

	m_arrComUI[eCOMPONENT_COLLIDER2D] = new CCollider2DUI;
	m_arrComUI[eCOMPONENT_COLLIDER2D]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[eCOMPONENT_COLLIDER2D]);

	m_arrComUI[eCOMPONENT_ANIMATOR2D] = new CAnimator2DUI;
	m_arrComUI[eCOMPONENT_ANIMATOR2D]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[eCOMPONENT_ANIMATOR2D]);

	m_arrComUI[eCOMPONENT_LIGHT2D] = new CLight2DUI;
	m_arrComUI[eCOMPONENT_LIGHT2D]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[eCOMPONENT_LIGHT2D]);

	m_arrComUI[eCOMPONENT_TILEMAP] = new CTilemapUI;
	m_arrComUI[eCOMPONENT_TILEMAP]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[eCOMPONENT_TILEMAP]);
}

CInspectorUI::~CInspectorUI()
{
	//Ŭ���� �ν��Ͻ� ������ Child UI�� ��ȸ �����ָ鼭 �����ǹǷ� ���⼭ �� �ʿ� ����.
}

void CInspectorUI::init()
{
	m_pTarget = CLevelMgr::GetInst()->FindObjectByName(L"Player");

	for (int i = 0; i < eCOMPONENT_END; ++i)
	{
		if (nullptr == m_arrComUI[i])
			continue;

		m_arrComUI[i]->SetTarget(m_pTarget);
	}
}

void CInspectorUI::tick()
{
	if (nullptr == m_pTarget)
		init();
	//���⼭�� ���߿� ���콺 Ŭ���� �޾Ƽ� Ŭ���� ��ġ�� �ִ� ������Ʈ�� �ּҸ� �������� �ڵ带 ����
}

int CInspectorUI::render_update()
{
	return TRUE;
}
