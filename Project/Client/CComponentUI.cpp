#include "pch.h"
#include "CComponentUI.h"

#include <Engine/CGameObject.h>
#include <Engine/func.h>

CComponentUI::CComponentUI(const string& _ID, eCOMPONENT_TYPE _Type)
	: CUI(_ID)
	, m_pTarget()
	, m_Type(_Type)
{
}

CComponentUI::~CComponentUI()
{
}

void CComponentUI::SetTarget(CGameObject* _pTarget)
{
	//타겟을 우선 대입
	m_pTarget = _pTarget;

	//자신이 담당하는 컴포넌트가 없을 경우 일단 표시 중지
	if (nullptr == m_pTarget || nullptr == m_pTarget->GetComponent(m_Type))
		SetActive(false);
	else
		SetActive(true);

}

bool CComponentUI::GetResKey(Ptr<CRes> _Res, __out string& _out)
{
	_out.clear();

	//Res 주소가 없다면 return
	if (nullptr == _Res)
		return false;

	const wstring& wstrKey = _Res->GetKey();

	return ::ConvertUnicodeToMultibyte(wstrKey, _out);
}

void CComponentUI::tick()
{
	if (nullptr != m_pTarget && nullptr != m_pTarget->GetComponent(m_Type))
		SetActive(true);
}

int CComponentUI::render_update()
{
	if (nullptr == m_pTarget || nullptr == m_pTarget->GetComponent(m_Type))
		return FALSE;

	ImGui::PushID(0);

	//버튼 생상값을 지정
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.6f));
	ImGui::Button(GetName().c_str());
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	return TRUE;
}
