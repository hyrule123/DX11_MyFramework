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
	//Ÿ���� �켱 ����
	m_pTarget = _pTarget;

	//�ڽ��� ����ϴ� ������Ʈ�� ���� ��� �ϴ� ǥ�� ����
	if (nullptr == m_pTarget || nullptr == m_pTarget->GetComponent(m_Type))
		SetActive(false);
	else
		SetActive(true);

}

const string& CComponentUI::GetResKey(Ptr<CRes> _Res)
{
	//Res �ּҰ� ���ٸ� return
	if (nullptr == _Res)
		return g_voidStr;

	return _Res->GetKey();
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

	//��ư ������ ����
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.6f));
	ImGui::Button(GetName().c_str());
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	return TRUE;
}
