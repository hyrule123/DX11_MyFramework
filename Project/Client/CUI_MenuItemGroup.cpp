
#include "pch.h"
#include "CUI_MenuItemGroup.h"

CUI_MenuItemGroup::CUI_MenuItemGroup()
	: CUI_BasicWindow("MenuItemGroup")
{

}

CUI_MenuItemGroup::~CUI_MenuItemGroup()
{
}


bool CUI_MenuItemGroup::beginUI()
{
	return true;
}

void CUI_MenuItemGroup::render_update()
{
	size_t size = m_vecItem.size();
	for (size_t i = 0; i < size; ++i)
	{

		if (ImGui::MenuItem(m_vecItem[i].Label.c_str(), m_vecItem[i].KeyBind.c_str(), &m_vecItem[i].bChecked, m_vecItem[i].bEnable))
		{
			if(nullptr != m_vecItem[i].Callback)
				m_vecItem[i].Callback();
		}

		if (false == m_vecItem[i].bCheckEnable)
			m_vecItem[i].bChecked = false;
	}
}

void CUI_MenuItemGroup::endUI()
{
}

void CUI_MenuItemGroup::AddItem(const tMenuItem& _Item)
{
	m_vecItem.push_back(_Item);
}

bool CUI_MenuItemGroup::SetCallback(const string& _sItemName, std::function<void()> _pCallbackFunc)
{
	tMenuItem* pMenuItem = FindItem(_sItemName);

	if (nullptr == pMenuItem)
		return false;

	pMenuItem->Callback = _pCallbackFunc;
	return true;
}

bool CUI_MenuItemGroup::SetEnable(const string& _sItemName, bool _bEnable)
{
	tMenuItem* pMenuItem = FindItem(_sItemName);
	if (nullptr == pMenuItem)
		return false;

	pMenuItem->bEnable = _bEnable;
	return true;
}

bool CUI_MenuItemGroup::SetItemSelected(const string& _sItemName, bool _bChecked)
{
	tMenuItem* pMenuItem = FindItem(_sItemName);
	if (nullptr == pMenuItem)
		return false;

	pMenuItem->bChecked = _bChecked;
	return true;
}

bool CUI_MenuItemGroup::GetItemSelected(const string& _sItemName)
{
	tMenuItem* pMenuItem = FindItem(_sItemName);

	if (nullptr == pMenuItem)
		return false;

	return pMenuItem->bChecked;
}

tMenuItem* CUI_MenuItemGroup::FindItem(const string& _sItemName)
{
	size_t size = m_vecItem.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (_sItemName == m_vecItem[i].Label)
			return &m_vecItem[i];
	}

	return nullptr;
}
