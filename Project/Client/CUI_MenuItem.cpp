
#include "pch.h"
#include "CUI_MenuItem.h"

CUI_MenuItem::CUI_MenuItem(const string& _strName, DWORD_PTR _pData)
	: CUI_Widget(_strName, eWIDGET_TYPE::MENU_ITEM)
	, m_bCheckEnable()
	, m_bChecked()
	, m_bEnable(true)
	, m_pData(_pData)
	, m_funcCallback()
{
}

CUI_MenuItem::~CUI_MenuItem()
{
}


bool CUI_MenuItem::beginUI()
{
	bool ret = ImGui::MenuItem(GetName().c_str(), m_strShortcutKeyBind.c_str(), &m_bChecked, m_bEnable);

	if (false == m_bCheckEnable)
		m_bChecked = false;
	
	return ret;
}

void CUI_MenuItem::render_update()
{
	if (nullptr != m_funcCallback)
		m_funcCallback(this, m_pData);
}


void CUI_MenuItem::SetCallback(std::function<void(CUI_MenuItem*, DWORD_PTR)> _pCallbackFunc)
{
	m_funcCallback = _pCallbackFunc;
}

