#include "pch.h"
#include "cUI_Button.h"


cUI_Button::cUI_Button(const string& _strButtonName)
	: cUI_Widget(_strButtonName, eWIDGET_TYPE::BUTTON)
	, m_funcCallback()
{
}

cUI_Button::~cUI_Button()
{
}

bool cUI_Button::beginUI()
{
	return ImGui::Button(GetKey().c_str(), m_vSize);
}

void cUI_Button::render_update()
{
	if (nullptr != m_funcCallback)
		m_funcCallback(GetUIData());
}

