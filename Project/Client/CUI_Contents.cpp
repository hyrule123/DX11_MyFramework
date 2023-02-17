#include "pch.h"


#include "CUI_Contents.h"
#include "CUI_Tree.h"

#include <Engine/CResMgr.h>

CUI_Contents::CUI_Contents()
	: CUI_BasicWindow("Contents")
	, m_pSelectedTree()
{
}

CUI_Contents::~CUI_Contents()
{
}


void CUI_Contents::init()
{
	for (int i = 0; i < (int)eRES_TYPE::END; ++i)
	{
		m_arrpResTypeRoot[i] = new CUI_Tree(g_eRES_TYPE_strName[i]);
		m_arrpResTypeRoot[i]->AddTreeFlag(ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen);
		AddChildUI(m_arrpResTypeRoot[i]);
	}
}

void CUI_Contents::tick()
{
	if (true == CResMgr::GetInst()->IsUpdated())
	{
		m_pSelectedTree = nullptr;
		UpdateResources();
	}
}

void CUI_Contents::ChangeSelectedTreeNode(CUI_Tree* _pTree, DWORD_PTR _pValue)
{
	if (nullptr == _pTree)
		return;
	
	if (nullptr != m_pSelectedTree)
	{
		m_pSelectedTree->SetSelected(false);
		m_pSelectedTree = nullptr;
	}
		
	m_pSelectedTree = _pTree;
	m_pSelectedTree->SetSelected(true);
}

void CUI_Contents::UpdateResources()
{
	for (int i = 0; i < (int)eRES_TYPE::END; ++i)
	{
		m_arrpResTypeRoot[i]->ClearChildUI();

		auto ResMap = CResMgr::GetInst()->GetResMap((eRES_TYPE)i);
		for (auto iter : ResMap)
		{
			CUI_Tree* Node = new CUI_Tree(iter.first, (DWORD_PTR)(iter.second.Get()));
			Node->SetFuncCallback(eUI_MOUSE_STATUS::LBTN_CLICKED, std::bind(&CUI_Contents::ChangeSelectedTreeNode, this, std::placeholders::_1, std::placeholders::_2));

			m_arrpResTypeRoot[i]->AddChildNode(Node);
		}

	}
}
