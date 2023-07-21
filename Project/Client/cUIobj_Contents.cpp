#include "pch.h"


#include "cUIobj_Contents.h"
#include "cUI_Tree.h"
#include "CUI_DragNDrop.h"

#include <Engine/cResMgr.h>
#include <Engine/strKey_Default.h>

cUIobj_Contents::cUIobj_Contents()
	: cUI_BasicWindow("Contents")
	, m_arrpResTypeSelected{}
{
}

cUIobj_Contents::~cUIobj_Contents()
{
}


void cUIobj_Contents::init()
{
	SetSaveEnable(true);

	for (int i = 0; i < (int)eRES_TYPE::END; ++i)
	{
		m_arrpResTypeRoot[i] = new cUI_Tree(string(DIRECTORY_NAME::RES_ARR[i]));
		m_arrpResTypeRoot[i]->AddTreeFlag(ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen);
		AddChildUI(m_arrpResTypeRoot[i]);
	}
}

void cUIobj_Contents::Tick()
{
	if (true == cResMgr::GetInst()->IsUpdated())
	{
		memset(m_arrpResTypeSelected, 0, sizeof(m_arrpResTypeSelected));
		UpdateResources();
	}
}

void cUIobj_Contents::ChangeSelectedTreeNode(tUIData _tUIData)
{
	if (nullptr == _tUIData.pUI || nullptr == _tUIData.tData.ptr)
		return;
	
	IRes* pRes = static_cast<IRes*>(_tUIData.tData.ptr);

	int iResTypeIdx = (int)pRes->GetResType();

	if (nullptr != m_arrpResTypeSelected[iResTypeIdx])
	{
		
		m_arrpResTypeSelected[iResTypeIdx]->SetSelected(false);
		m_arrpResTypeSelected[iResTypeIdx] = nullptr;
	}
		

	cUI_Tree* pTree = (cUI_Tree*)(_tUIData.pUI);
	m_arrpResTypeSelected[iResTypeIdx] = (cUI_Tree*)(_tUIData.pUI);
	m_arrpResTypeSelected[iResTypeIdx]->SetSelected(true);
}

void cUIobj_Contents::UpdateResources()
{
	for (int i = 0; i < (int)eRES_TYPE::END; ++i)
	{
		m_arrpResTypeRoot[i]->ClearChildUI();

		auto ResMap = cResMgr::GetInst()->GetResMap((eRES_TYPE)i);
		for (auto iter : ResMap)
		{
			cUI_Tree* Node = new cUI_Tree(iter.first);
			tPtrData data = { iter.second.Get(), };
			Node->SetDataPtr(data);
			Node->SetFuncCallback(eUI_MOUSE_STATUS::LBTN_CLICKED, std::bind(&cUIobj_Contents::ChangeSelectedTreeNode, this, std::placeholders::_1));

			//CUI_DragNDropSender* dndSender = new CUI_DragNDropSender(string(DIRECTORY_NAME::RES_ARR[i]));
			//dndSender->SetDataPtr(data);
			//Node->AddChildUI(dndSender);

			m_arrpResTypeRoot[i]->AddChildNode(Node);
		}
	}
}
