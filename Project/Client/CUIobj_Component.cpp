#include "pch.h"
#include "CUIobj_Component.h"

#include <Engine/CGameObject.h>
#include <Engine/func.h>

#include "macroFunc_Imgui.h"

CUIobj_Component::CUIobj_Component(const string& _strName, eCOMPONENT_TYPE _Type)
	: CUI_BasicWindow(_strName)
	, m_pTarget()
	, m_Type(_Type)
{
}

CUIobj_Component::~CUIobj_Component()
{
}

void CUIobj_Component::SetTarget(CGameObject* _pTarget)
{
	//타겟을 우선 대입
	m_pTarget = _pTarget;

	////자신이 담당하는 컴포넌트가 없을 경우 일단 표시 중지
	//if (nullptr == m_pTarget || nullptr == m_pTarget->GetComponent(m_Type))
	//	SetActive(false);
	//else
	//	SetActive(true);

}

CComponent* CUIobj_Component::GetMyTargetComponent()
{
	if (nullptr == m_pTarget)
		return nullptr;
	
	return m_pTarget->GetComponent(m_Type);
}

const string& CUIobj_Component::GetResKey(Ptr<CRes> _Res)
{
	//Res 주소가 없다면 return
	if (nullptr == _Res)
		return g_voidStr;

	return _Res->GetKey();
}


bool CUIobj_Component::beginUI()
{
	//아예 타겟 오브젝트가 없을경우에는 출력 안함
	if (nullptr == m_pTarget)
		return false;

	//타겟 오브젝트가 있을 경우에는 컴포넌트 구분을 표시
	IMGUI_Indicator(GetName().c_str(), ImColorPreset::DarkRed);

	CUI_BasicWindow::beginUI();
	
	return true;
}

void CUIobj_Component::render_update()
{
	//타겟 오브젝트가 있을 경우에는 새 컴포넌트 생성 알림 추가
	if (nullptr == m_pTarget->GetComponent(m_Type))
	{
		ImGui::Text("Empty Component");

		CreateNewComUI();

		ImGui::NewLine();
	}
	else
	{
		EditComUI();
	}
	
}

void CUIobj_Component::endUI()
{
	CUI_BasicWindow::endUI();
}

