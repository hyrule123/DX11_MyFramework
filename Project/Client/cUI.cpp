#include "pch.h"
#include "cUI.h"


#include "cImGuiMgr.h"

cUI::cUI(const string& _Name)
	: IEntity(_Name)
	, m_ParentUI()
	, m_strID(_Name)
	, m_bActive(true)
	, m_bNoChildUI()
	, m_tData{ this, }
	, m_bSaveEnable()
{
}

cUI::~cUI()
{
	size_t size = m_vecChildUI.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (nullptr != m_vecChildUI[i])
			delete(m_vecChildUI[i]);
	}

	//cImGuiMgr::GetInst()->RemoveUI(GetStrID());
}




void cUI::tickRecursive()
{
	Tick();

	size_t size = m_vecChildUI.size();
	for (size_t i = 0; i < size; i++)
	{
		m_vecChildUI[i]->tickRecursive();
	}
}


void cUI::FinalTick()
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
			m_vecChildUI[i]->FinalTick();
		}

		ImGui::Separator();

		endUI();
	}
}

void cUI::ManualUpdate()
{
	FinalTick();
}

void cUI::SaveRecursive(Json::Value& _Node)
{
	if(true == m_bSaveEnable)
		Save(_Node);

	size_t size = m_vecChildUI.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChildUI[i]->SaveRecursive(_Node);
	}
}

void cUI::LoadRecursive(Json::Value& _Node)
{
	Load(_Node);

	size_t size = m_vecChildUI.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChildUI[i]->LoadRecursive(_Node);
	}
}



void cUI::MakeUniqueID()
{
	m_strID += "##";
	m_strID += std::to_string(GetID());
}

void cUI::MakeUniqueID(const string& _strUniqueIDSuffix)
{
	if (true == _strUniqueIDSuffix.empty())
		return;

	m_strID += "##";
	m_strID += _strUniqueIDSuffix;
}

void cUI::AddChildUI(cUI* _UI)
{
	if (true == m_bNoChildUI)
	{
		SAFE_DELETE(_UI);
		return ;
	}
		

	_UI->m_ParentUI = this;
	m_vecChildUI.push_back(_UI);

	cImGuiMgr::GetInst()->AddUI(_UI);
}



cUI* cUI::FindChildUIByName(const string& _Name)
{
	size_t size = m_vecChildUI.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (_Name == m_vecChildUI[i]->GetKey())
			return m_vecChildUI[i];
	}

	return nullptr;
}

void cUI::ClearChildUI()
{
	size_t size = m_vecChildUI.size();
	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(m_vecChildUI[i]);
	}
	m_vecChildUI.clear();
}



