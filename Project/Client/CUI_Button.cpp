#include "pch.h"
#include "CUI_Button.h"


CUI_Button::CUI_Button(const string& _strButtonName)
	: CUI_Widget(_strButtonName, eWIDGET_TYPE::BUTTON)
	, m_funcCallback()
{
}

CUI_Button::~CUI_Button()
{
}

bool CUI_Button::beginUI()
{
	return ImGui::Button(GetName().c_str(), m_vSize);
}

void CUI_Button::render_update()
{
	if (nullptr != m_funcCallback)
		m_funcCallback(GetUIData());
}

