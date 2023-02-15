#include "pch.h"
#include "CUI_Component.h"

#include <Engine/CGameObject.h>
#include <Engine/func.h>

CUI_Component::CUI_Component(const string& _strName, eCOMPONENT_TYPE _Type)
	: CUI_BasicWindow(_strName)
	, m_pTarget()
	, m_Type(_Type)
{
}

CUI_Component::~CUI_Component()
{
}

void CUI_Component::SetTarget(CGameObject* _pTarget)
{
	//타겟을 우선 대입
	m_pTarget = _pTarget;

	//자신이 담당하는 컴포넌트가 없을 경우 일단 표시 중지
	if (nullptr == m_pTarget || nullptr == m_pTarget->GetComponent(m_Type))
		SetActive(false);
	else
		SetActive(true);

}

const string& CUI_Component::GetResKey(Ptr<CRes> _Res)
{
	//Res 주소가 없다면 return
	if (nullptr == _Res)
		return g_voidStr;

	return _Res->GetKey();
}

void CUI_Component::tick()
{
	if (nullptr != m_pTarget && nullptr != m_pTarget->GetComponent(m_Type))
		SetActive(true);
}

bool CUI_Component::beginUI()
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

void CUI_Component::render_update()
{

}

void CUI_Component::endUI()
{
	CUI_BasicWindow::endUI();
}

