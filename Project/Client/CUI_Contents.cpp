#include "pch.h"


#include "CUI_Contents.h"
#include "CUI_Tree.h"

#include <Engine/CResMgr.h>

CUI_Contents::CUI_Contents()
	: CUI_BasicWindow("Contents")
	, m_arrpResTypeSelected{}
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
		memset(m_arrpResTypeSelected, 0, sizeof(m_arrpResTypeSelected));
		UpdateResources();
	}
}

void CUI_Contents::ChangeSelectedTreeNode(tUIData _tUIData)
{
	if (nullptr == _tUIData.pUI || nullptr == _tUIData.tData.ptr)
		return;
	
	CRes* pRes = static_cast<CRes*>(_tUIData.tData.ptr);

	int iResTypeIdx = (int)pRes->GetResType();

	if (nullptr != m_arrpResTypeSelected[iResTypeIdx])
	{
		
		m_arrpResTypeSelected[iResTypeIdx]->SetSelected(false);
		m_arrpResTypeSelected[iResTypeIdx] = nullptr;
	}
		

	CUI_Tree* pTree = (CUI_Tree*)(_tUIData.pUI);
	m_arrpResTypeSelected[iResTypeIdx] = (CUI_Tree*)(_tUIData.pUI);
	m_arrpResTypeSelected[iResTypeIdx]->SetSelected(true);
}

void CUI_Contents::UpdateResources()
{
	for (int i = 0; i < (int)eRES_TYPE::END; ++i)
	{
		m_arrpResTypeRoot[i]->ClearChildUI();

		auto ResMap = CResMgr::GetInst()->GetResMap((eRES_TYPE)i);
		for (auto iter : ResMap)
		{
			CUI_Tree* Node = new CUI_Tree(iter.first);
			Node->SetDataPtr(tPtrData{ iter.second.Get(), });
			Node->SetFuncCallback(eUI_MOUSE_STATUS::LBTN_CLICKED, std::bind(&CUI_Contents::ChangeSelectedTreeNode, this, std::placeholders::_1));

			m_arrpResTypeRoot[i]->AddChildNode(Node);
		}
		
	}
}
