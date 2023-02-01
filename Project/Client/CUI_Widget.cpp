#include "pch.h"
#include "CUI_Widget.h"

CUI_Widget::CUI_Widget(const string& _ID, eWIDGET_TYPE _Type)
	: CUI_BasicWindow(_ID)
	, m_Type(_Type)
	, m_bLeftLabel(true)
	, m_fLeftLabelWidth(100.f)
	, m_bSizeSet()
{
}

CUI_Widget::~CUI_Widget()
{
}

bool CUI_Widget::beginUI()
{
	//위젯은 반드시 부모 객체가 있어야 한다.
	assert(nullptr != CUI_BasicWindow::GetParent());

	//width를 설정해놓았을 경우
	float xSize = GetWidth();
	if (0.f != xSize)
	{
		ImGui::PushItemWidth(xSize);
		m_bSizeSet = true;
	}
	else
		m_bSizeSet = false;
		

	if (true == m_bLeftLabel)
	{
		ImGui::Text(GetName().data());
		ImGui::SameLine(m_fLeftLabelWidth);
	}

	return true;
}

void CUI_Widget::endUI()
{
	if (true == m_bSizeSet)
		ImGui::PopItemWidth();
}
