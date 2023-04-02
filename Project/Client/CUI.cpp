#include "pch.h"
#include "CUI.h"


#include "CImGuiMgr.h"

CUI::CUI(const string& _Name)
	: CEntity(_Name)
	, m_ParentUI()
	, m_strID(_Name)
	, m_bActive(true)
	, m_bNoChildUI()
	, m_tData{ this, }
	, m_bSaveEnable()
{
}

CUI::~CUI()
{
	size_t size = m_vecChildUI.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (nullptr != m_vecChildUI[i])
			delete(m_vecChildUI[i]);
	}

	CImGuiMgr::GetInst()->RemoveUI(GetStrID());
}




void CUI::tickRecursive()
{
	tick();

	size_t size = m_vecChildUI.size();
	for (size_t i = 0; i < size; i++)
	{
		m_vecChildUI[i]->tickRecursive();
	}
}


void CUI::finaltick()
{
	if (false == m_bActive)
		return;

	if (true == beginUI())
	{
		render_update();

		size_t size = m_vecChildUI.size();
		for (size_t i = 0; i < size; ++i)
		{
			//자식 UI를 자신의 UI 위쪽에 렌더링한다.
			m_vecChildUI[i]->finaltick();
		}

		ImGui::Separator();

		endUI();
	}


}

void CUI::SaveRecursive(Json::Value& _Node)
{
	if(true == m_bSaveEnable)
		Save(_Node);

	size_t size = m_vecChildUI.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChildUI[i]->SaveRecursive(_Node);
	}
}

void CUI::LoadRecursive(Json::Value& _Node)
{
	Load(_Node);

	size_t size = m_vecChildUI.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChildUI[i]->LoadRecursive(_Node);
	}
}



void CUI::MakeUniqueID()
{
	m_strID += "##";
	m_strID += std::to_string(GetID());
}

void CUI::MakeUniqueID(const string& _strUniqueIDSuffix)
{
	if (true == _strUniqueIDSuffix.empty())
		return;

	m_strID += "##";
	m_strID += _strUniqueIDSuffix;
}

void CUI::AddChildUI(CUI* _UI)
{
	if (true == m_bNoChildUI)
	{
		SAFE_DELETE(_UI);
		return ;
	}
		

	_UI->m_ParentUI = this;
	m_vecChildUI.push_back(_UI);

	CImGuiMgr::GetInst()->AddUI(_UI);
}



CUI* CUI::FindChildUIByName(const string& _Name)
{
	size_t size = m_vecChildUI.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (_Name == m_vecChildUI[i]->GetName())
			return m_vecChildUI[i];
	}

	return nullptr;
}

void CUI::ClearChildUI()
{
	size_t size = m_vecChildUI.size();
	for (size_t i = 0; i < size; ++i)
	{
		DESTRUCTOR_DELETE(m_vecChildUI[i]);
	}
	m_vecChildUI.clear();
}



