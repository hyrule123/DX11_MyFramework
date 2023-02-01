#include "pch.h"
#include "CUI_Widget.h"

CUI_Widget::CUI_Widget(const string& _ID, eWIDGET_TYPE _Type)
	: CUI(_ID)
	, m_Type(_Type)
	, m_bLeftLabel(true)
	, m_fLeftLabelWidth(100.f)
{
}

CUI_Widget::~CUI_Widget()
{
}

void CUI_Widget::finaltick()
{
	//������ �ݵ�� �θ� ��ü�� �־�� �Ѵ�.
	assert(nullptr != CUI::GetParent());

	bool sizeset = (0.f != GetSize().x);
	if (true == sizeset)
		ImGui::PushItemWidth(GetSize().x);

	if (true == m_bLeftLabel)
	{
		ImGui::Text(GetName().c_str());
		ImGui::SameLine(m_fLeftLabelWidth);
	}

	render_update();

	if (sizeset)
		ImGui::PopItemWidth();

}