#include "pch.h"


#include "CUIobj_Contents.h"
#include "CUI_Tree.h"
#include "CUI_DragNDrop.h"

#include <Engine/CResMgr.h>
#include <Engine/strKey_Default.h>

CUIobj_Contents::CUIobj_Contents()
	: CUI_BasicWindow("Contents")
	, m_arrpResTypeSelected{}
{
}

CUIobj_Contents::~CUIobj_Contents()
{
}


void CUIobj_Contents::init()
{
	SetSaveEnable(true);

	for (int i = 0; i < (int)eRES_TYPE::END; ++i)
	{
		m_arrpResTypeRoot[i] = new CUI_Tree(string(DIRECTORY_NAME::RES_ARR[i]));
		m_arrpResTypeRoot[i]->AddTreeFlag(ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen);
		AddChildUI(m_arrpResTypeRoot[i]);
	}
}

void CUIobj_Contents::tick()
{
	if (true == CResMgr::GetInst()->IsUpdated())
	{
		memset(m_arrpResTypeSelected, 0, sizeof(m_arrpResTypeSelected));
		UpdateResources();
	}
}

void CUIobj_Contents::ChangeSelectedTreeNode(tUIData _tUIData)
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

void CUIobj_Contents::UpdateResources()
{
	for (int i = 0; i < (int)eRES_TYPE::END; ++i)
	{
		m_arrpResTypeRoot[i]->ClearChildUI();

		auto ResMap = CResMgr::GetInst()->GetResMap((eRES_TYPE)i);
		for (auto iter : ResMap)
		{
			CUI_Tree* Node = new CUI_Tree(iter.first);
			tPtrData data = { iter.second.Get(), };
			Node->SetDataPtr(data);
			Node->SetFuncCallback(eUI_MOUSE_STATUS::LBTN_CLICKED, std::bind(&CUIobj_Contents::ChangeSelectedTreeNode, this, std::placeholders::_1));

			//CUI_DragNDropSender* dndSender = new CUI_DragNDropSender(string(DIRECTORY_NAME::RES_ARR[i]));
			//dndSender->SetDataPtr(data);
			//Node->AddChildUI(dndSender);

			m_arrpResTypeRoot[i]->AddChildNode(Node);
		}
	}
}
