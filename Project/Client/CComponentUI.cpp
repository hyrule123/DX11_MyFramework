#include "pch.h"
#include "CComponentUI.h"

#include <Engine/CGameObject.h>

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

void CComponentUI::GetResKey(Ptr<CRes> _Res, char* _Buff, size_t _BufferSize)
{
	//Res �ּҰ� ���ٸ� return
	if (nullptr == _Res)
		return;

	memset(_Buff, 0, sizeof(char) * _BufferSize);

	wstring wstrKey = _Res->GetKey();
	string strKey = string(wstrKey.begin(), wstrKey.end());

	size_t len = strKey.length();
	if (len > _BufferSize)
		return;

	memcpy(_Buff, strKey.data(), sizeof(char) * len);
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
