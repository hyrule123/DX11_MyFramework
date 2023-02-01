
#include "pch.h"
#include "CUI_BasicWindow.h"



#include "CUI_Widget.h"

CUI_BasicWindow::CUI_BasicWindow(const string& _Name) 
	: CUI(_Name)
	, m_WindowFlag()

{
}


CUI_BasicWindow::~CUI_BasicWindow()
{

}


bool CUI_BasicWindow::beginUI()
{
	if (nullptr == GetParent())
	{
		return ImGui::Begin(GetstrID().c_str(), GetActivePtr(), m_WindowFlag);
	}


	ImGui::BeginChild(GetstrID().c_str(), m_vSize, m_bBorder, m_WindowFlag);
	return true;
}

void CUI_BasicWindow::endUI()
{
	if (nullptr == GetParent())
	{
		ImGui::End();
		return;
	}
	
	ImGui::EndChild();
}
