#include "pch.h"
#include "CUI_Outliner.h"

#include "CUI_Tree.h"

#include "CImGuiMgr.h"
#include "CUI_Inspector.h"

#include <Engine/CEventMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>


CUI_Outliner::CUI_Outliner()
	: CUI_BasicWindow("Outliner")
	, m_arrLayer{}
	, m_pInspectorUI()
{

}

CUI_Outliner::~CUI_Outliner()
{

}

void CUI_Outliner::init()
{
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurLevel();

	if (nullptr == pLevel)
		return;

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pLevel->GetLayer((UINT)i);

		string strLayerName = pLayer->GetName();

		if (true == strLayerName.empty())
			strLayerName = "No Name";

		CUI_Tree* pTree = new CUI_Tree(strLayerName);
		pTree->AddTreeFlag(ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen);

		AddChildUI(pTree);

		m_arrLayer[i] = pTree;
	}

	

	m_pInspectorUI = static_cast<CUI_Inspector*>(CImGuiMgr::GetInst()->FindUI("Inspector"));
}

void CUI_Outliner::tick()
{
	if (true == CEventMgr::GetInst()->IsLevelChanged())
	{
		UpdateObjectTree();
	}
}

void CUI_Outliner::UpdateObjectTree()
{
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurLevel();

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->ClearChildUI();

		CLayer* pLayer = pLevel->GetLayer((UINT)i);

		const auto& vecObj = pLayer->GetvecObj();
		size_t size = vecObj.size();
		for (size_t j = 0; j < size; ++j)
		{
			if (nullptr != vecObj[j]->GetParent())
				continue;
			
			LoadGameObjectHierarchy(m_arrLayer[i], vecObj[j]);
		}
	}
}

void CUI_Outliner::LoadGameObjectHierarchy(CUI_Tree* _pUI, CGameObject* _pGameObject)
{

	CUI_Tree* pTree = new CUI_Tree(_pGameObject->GetName());
	tPtrData ptrData{ _pGameObject, };

	pTree->SetFuncCallback(eUI_MOUSE_STATUS::LBTN_DOUBLE_CLICKED,
		std::bind(&CUI_Outliner::CallbackLinkInspector, this, std::placeholders::_1));
	pTree->AddTreeFlag(ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen);

	_pUI->AddChildUI(pTree);

	size_t numChild = _pGameObject->GetNumChilds();
	if ((size_t)0 == numChild)
		return;

	const vector<CGameObject*>& vecChild = _pGameObject->GetvecChilds();
	for (size_t i = 0; i < numChild; ++i)
	{
		LoadGameObjectHierarchy(pTree, vecChild[i]);
	}
}

void CUI_Outliner::CallbackLinkInspector(tUIData _UIData)
{
	if (nullptr == _UIData.tData.ptr)
		return;

	m_pInspectorUI->SetTarget(static_cast<CGameObject*>(_UIData.tData.ptr));
}
