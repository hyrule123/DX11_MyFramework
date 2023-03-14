#include "pch.h"
#include "CUIobj_Component.h"

#include <Engine/CGameObject.h>
#include <Engine/func.h>

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

	//자신이 담당하는 컴포넌트가 없을 경우 일단 표시 중지
	if (nullptr == m_pTarget || nullptr == m_pTarget->GetComponent(m_Type))
		SetActive(false);
	else
		SetActive(true);

}

const string& CUIobj_Component::GetResKey(Ptr<CRes> _Res)
{
	//Res 주소가 없다면 return
	if (nullptr == _Res)
		return g_voidStr;

	return _Res->GetKey();
}

void CUIobj_Component::tick()
{
	if (nullptr != m_pTarget && nullptr != m_pTarget->GetComponent(m_Type))
		SetActive(true);
}

bool CUIobj_Component::beginUI()
{
	//타겟 게임오브젝트가 존재하기 않거나 자신의 담당 컴포넌트가 존재하지 않을 시 출력하지 않는다.
	if (nullptr == m_pTarget || nullptr == m_pTarget->GetComponent(m_Type))
		return false;

	ImGui::PushID(0);

	//버튼 생상값을 지정
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.6f));
	ImGui::Button(GetStrID().c_str());
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	CUI_BasicWindow::beginUI();

	return true;
}

void CUIobj_Component::render_update()
{

}

void CUIobj_Component::endUI()
{
	CUI_BasicWindow::endUI();
}

