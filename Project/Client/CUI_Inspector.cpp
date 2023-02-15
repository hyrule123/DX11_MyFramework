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
	//Ŭ���� �ν��Ͻ� ������ Child UI�� ��ȸ �����ָ鼭 �����ǹǷ� ���⼭ �� �ʿ� ����.
}

void CUI_Inspector::init()
{
	m_pTarget = CLevelMgr::GetInst()->FindObjectByName("ParticleObj");

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
	//�� tick���� ������Ʈ�� ���¸� Ȯ���Ͽ� ����
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


	//���⼭�� ���߿� ���콺 Ŭ���� �޾Ƽ� Ŭ���� ��ġ�� �ִ� ������Ʈ�� �ּҸ� �������� �ڵ带 ����
	CKeyMgr* KeyMgr = CKeyMgr::GetInst();
	if (KEY_STATE::TAP == KeyMgr->GetKeyState(KEY::LBTN))
	{
		//���콺 ��ġ�� �޾ƿ´�. �� ��ǥ�� NDC ��ǥ��� �ݴ��̹Ƿ� y���� ������ ���ش�.
		Vec3 vMouseWorldPos(KeyMgr->GetMousePos(), 0.f);
		vMouseWorldPos.y = g_GlobalVal.vResolution.y - vMouseWorldPos.y;

		//NDC ��ǥ��� ������ �����̴�. -> �ػ��� �ݸ�ŭ ���ش�.
		vMouseWorldPos.x -= g_GlobalVal.vResolution.x * 0.5f;
		vMouseWorldPos.y -= g_GlobalVal.vResolution.y * 0.5f;

		CCamera* pCam = CRenderMgr::GetInst()->GetCurCamera();
		const Vec3& CamPos = pCam->Transform()->GetWorldPos();

		//���� ��ǥ�� ī�޶��� ��ġ�� ���Ѵ�. -> ����󿡼��� ��ǥ�� ��ȯ �Ϸ�.
		vMouseWorldPos += CamPos;

		//�ش� ���⿡�� ī�޶� ���� �������� ������ ������ ����� �浹�˻縦 �ǽ��Ѵ�.

		

		//���콺 ��ġ�� 
		

		//���콺 ��ġ�� ���Ѵ�.
		//vMouseWorldPos.x += MousePos.x;
		//vMouseWorldPos.y -= MousePos.y;
		
		//���콺 ��ǥ�� NDC ��ǥ �������� ��ȯ�Ѵ�.(�ػ� �� ���Ѱ� ����)





 
		

		//Vec4 vMousePos = Vec4(MousePos.x, MousePos.y, 0.f, 1.f);


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

