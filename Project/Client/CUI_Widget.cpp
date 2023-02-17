#include "pch.h"
#include "CUI_Widget.h"

CUI_Widget::CUI_Widget(const string& _strName, eWIDGET_TYPE _Type)
	: CUI_BasicWindow(_strName)
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
	//������ �ݵ�� �θ� ��ü�� �־�� �Ѵ�.
	assert(nullptr != CUI_BasicWindow::GetParent());

	//width�� �����س����� ���
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
		ImGui::Text(GetStrID().data());
		ImGui::SameLine(m_fLeftLabelWidth);
	}

	return true;
}

void CUI_Widget::render_update()
{
}

void CUI_Widget::endUI()
{
	if (true == m_bSizeSet)
		ImGui::PopItemWidth();
}
