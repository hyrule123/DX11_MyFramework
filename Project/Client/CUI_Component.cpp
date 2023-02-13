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
	//Ÿ���� �켱 ����
	m_pTarget = _pTarget;

	//�ڽ��� ����ϴ� ������Ʈ�� ���� ��� �ϴ� ǥ�� ����
	if (nullptr == m_pTarget || nullptr == m_pTarget->GetComponent(m_Type))
		SetActive(false);
	else
		SetActive(true);

}

const string& CUI_Component::GetResKey(Ptr<CRes> _Res)
{
	//Res �ּҰ� ���ٸ� return
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
	//Ÿ�� ���ӿ�����Ʈ�� �����ϱ� �ʰų� �ڽ��� ��� ������Ʈ�� �������� ���� �� ������� �ʴ´�.
	if (nullptr == m_pTarget || nullptr == m_pTarget->GetComponent(m_Type))
		return false;

	ImGui::PushID(0);

	//��ư ������ ����
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.6f));
	ImGui::Button(GetName().c_str());
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

