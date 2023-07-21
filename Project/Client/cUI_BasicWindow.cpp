
#include "pch.h"
#include "cUI_BasicWindow.h"

#include "cUI_Widget.h"

#include <Engine/jsoncpp.h>


cUI_BasicWindow::cUI_BasicWindow(const string& _Name)
	: cUI(_Name)
	, m_WindowFlag()
	, m_bBorder()
{
}


cUI_BasicWindow::~cUI_BasicWindow()
{

}


bool cUI_BasicWindow::beginUI()
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


void cUI_BasicWindow::endUI()
{
	if (nullptr == GetParent())
	{
		ImGui::End();
		return;
	}
	
	ImGui::EndChild();
}

void cUI_BasicWindow::Save(Json::Value& _Save)
{
	if (nullptr != GetParent())
		return;

	_Save[GetStrID()]["IsActive"] = GetActive();
}

void cUI_BasicWindow::Load(Json::Value& _Load)
{
	if (nullptr != GetParent())
		return;

	if (_Load[GetStrID()])
	{
		SetActive(_Load[GetStrID()]["IsActive"].as<bool>());
	}
}
