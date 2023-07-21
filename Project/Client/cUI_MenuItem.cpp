
#include "pch.h"
#include "cUI_MenuItem.h"

cUI_MenuItem::cUI_MenuItem(const string& _strName, tPtrData _pData)
	: cUI_Widget(_strName, eWIDGET_TYPE::MENU_ITEM)
	, m_bCheckEnable()
	, m_bChecked()
	, m_bEnable(true)
	, m_pData(_pData)
	, m_funcCallback()
{
}

cUI_MenuItem::~cUI_MenuItem()
{
}


bool cUI_MenuItem::beginUI()
{
	bool ret = ImGui::MenuItem(GetKey().c_str(), m_strShortcutKeyBind.c_str(), &m_bChecked, m_bEnable);

	if (false == m_bCheckEnable)
		m_bChecked = false;
	
	return ret;
}

void cUI_MenuItem::render_update()
{
	if (nullptr != m_funcCallback)
		m_funcCallback(this, m_pData);
}


void cUI_MenuItem::SetCallback(std::function<void(cUI_MenuItem*, tPtrData)> _pCallbackFunc)
{
	m_funcCallback = _pCallbackFunc;
}

