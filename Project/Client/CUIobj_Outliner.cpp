#include "pch.h"
#include "CUIobj_Outliner.h"

#include "CUI_Tree.h"

#include "CImGuiMgr.h"
#include "CUIobj_Inspector.h"

#include <Engine/CEventMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>


CUIobj_Outliner::CUIobj_Outliner()
	: CUI_BasicWindow("Outliner")
	, m_arrLayer{}
	, m_pInspectorUI()
{

}

CUIobj_Outliner::~CUIobj_Outliner()
{

}

void CUIobj_Outliner::init()
{
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurLevel();

	if (nullptr == pLevel)
		return;

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		CLayer& pLayer = pLevel->GetLayer((UINT)i);

		string strLayerName = "Layer ";
		strLayerName += std::to_string(i) + " \"";

		if (true == pLayer.GetName().empty())
			strLayerName += "No Name";
		else
			strLayerName += pLayer.GetName();

		strLayerName += "\"";

		CUI_Tree* pTree = new CUI_Tree(strLayerName);
		pTree->AddTreeFlag(ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen);

		AddChildUI(pTree);

		m_arrLayer[i] = pTree;
	}

	m_pInspectorUI = static_cast<CUIobj_Inspector*>(CImGuiMgr::GetInst()->FindUI("Inspector"));
}

void CUIobj_Outliner::tick()
{
	if (true == CEventMgr::GetInst()->IsLevelChanged())
	{
		UpdateObjectTree();
	}
}

void CUIobj_Outliner::UpdateObjectTree()
{
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurLevel();

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->ClearChildUI();

		CLayer& pLayer = pLevel->GetLayer((UINT)i);

		const auto& vecObj = pLayer.GetvecObj();
		size_t size = vecObj.size();
		for (size_t j = 0; j < size; ++j)
		{
			//자식 오브젝트는 continue
			if (nullptr != vecObj[j]->GetParent())
				continue;
			
			//최상위 부모 오브젝트인 경우에만 계층구조로 로드한다.
			LoadGameObjectHierarchy(m_arrLayer[i], vecObj[j]);
		}
	}
}

void CUIobj_Outliner::LoadGameObjectHierarchy(CUI_Tree* _pUI, CGameObject* _pGameObject)
{
	if (nullptr == _pGameObject)
		return;

	CUI_Tree* pTree = new CUI_Tree(_pGameObject->GetName());

	pTree->SetDataPtr(tPtrData{ _pGameObject, });
	
	pTree->SetFuncCallback(eUI_MOUSE_STATUS::LBTN_DOUBLE_CLICKED,
		std::bind(&CUIobj_Outliner::CallbackLinkInspector, this, std::placeholders::_1));

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

void CUIobj_Outliner::CallbackLinkInspector(tUIData _UIData)
{
	if (nullptr == _UIData.tData.ptr)
		return;

	m_pInspectorUI->SetTarget(static_cast<CGameObject*>(_UIData.tData.ptr));
}
