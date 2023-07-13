
#include "pch.h"
#include "CUI_BasicWindow.h"

#include "CUI_Widget.h"

#include <Engine/jsoncpp.h>


CUI_BasicWindow::CUI_BasicWindow(const string& _Name)
	: CUI(_Name)
	, m_WindowFlag()
	, m_bBorder()
{
}


CUI_BasicWindow::~CUI_BasicWindow()
{

}


bool CUI_BasicWindow::beginUI()
{
	
	if (nullptr == GetParent())
	{
		if (false == ImGui::Begin(GetStrID().c_str(), GetActivePtr(), m_WindowFlag))
		{
			ImGui::End();
			return false;
		}	
		else
			return true;
	}

	
	return ImGui::BeginChild(GetStrID().c_str(), m_vSize, m_bBorder, m_WindowFlag);
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

void CUI_BasicWindow::Save(Json::Value& _Save)
{
	if (nullptr != GetParent())
		return;

	_Save[GetStrID()]["IsActive"] = GetActive();
}

void CUI_BasicWindow::Load(Json::Value& _Load)
{
	if (nullptr != GetParent())
		return;

	if (_Load[GetStrID()])
	{
		SetActive(_Load[GetStrID()]["IsActive"].as<bool>());
	}
}
