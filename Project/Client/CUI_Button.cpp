#include "pch.h"
#include "CUI_Button.h"


CUI_Button::CUI_Button(const string& _strButtonName)
	: CUI_Widget(_strButtonName, eWIDGET_TYPE::BUTTON)
{
}

CUI_Button::~CUI_Button()
{
}

bool CUI_Button::beginUI()
{
	return ImGui::Button(GetName().c_str(), GetSize());
}

void CUI_Button::render_update()
{
}

void CUI_Button::endUI()
{
}
