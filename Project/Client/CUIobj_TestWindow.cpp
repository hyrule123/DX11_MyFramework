#include "pch.h"
#include "CUIobj_TestWindow.h"

#include "CUI_Button.h"
#include "CUI_ComboBox.h"

CUIobj_TestWindow::CUIobj_TestWindow()
	: CUI_BasicWindow("TestWindow")
{
	SetSaveEnable(true);
}

CUIobj_TestWindow::~CUIobj_TestWindow()
{
}

void CUIobj_TestWindow::init()
{
    AddChildUI(new CUI_Button("TestButton"));

    CUI_Button* pButton = (CUI_Button*)FindChildUIByName("TestButton");

	CUI_ComboBox* pCombo = new CUI_ComboBox("MapChange");

	pCombo->SetWidth(200.f);
	pCombo->SetLeftLabel(true);
	pCombo->SetLeftLabelWidth(100.f);


	AddChildUI(pCombo);
}

void CUIobj_TestWindow::render_update()
{

}
