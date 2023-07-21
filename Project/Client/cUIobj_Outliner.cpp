#include "pch.h"
#include "cUIobj_Outliner.h"

#include "cUI_Tree.h"

#include "cImGuiMgr.h"
#include "cUIobj_Inspector.h"

#include <Engine/cEventMgr.h>
#include <Engine/cLevelMgr.h>
#include <Engine/cLevel.h>
#include <Engine/cLayer.h>
#include <Engine/cGameObject.h>


cUIobj_Outliner::cUIobj_Outliner()
	: cUI_BasicWindow("Outliner")
	, m_arrLayer{}
	, m_pInspectorUI()
{

}

cUIobj_Outliner::~cUIobj_Outliner()
{

}

void cUIobj_Outliner::init()
{
	cLevel* pLevel = cLevelMgr::GetInst()->GetCurLevel();

	if (nullptr == pLevel)
		return;

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		cLayer& pLayer = pLevel->GetLayer((UINT)i);

		string strLayerName = "Layer ";
		strLayerName += std::to_string(i) + " \"";

		if (true == pLayer.GetKey().empty())
			strLayerName += "No Name";
		else
			strLayerName += pLayer.GetKey();

		strLayerName += "\"";

		cUI_Tree* pTree = new cUI_Tree(strLayerName);
		pTree->AddTreeFlag(ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen);

		AddChildUI(pTree);

		m_arrLayer[i] = pTree;
	}

	m_pInspectorUI = static_cast<cUIobj_Inspector*>(cImGuiMgr::GetInst()->FindUI("Inspector"));
}

void cUIobj_Outliner::Tick()
{
	if (true == cEventMgr::GetInst()->IsLevelChanged())
	{
		UpdateObjectTree();
	}
}

void cUIobj_Outliner::UpdateObjectTree()
{
	cLevel* pLevel = cLevelMgr::GetInst()->GetCurLevel();

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->ClearChildUI();

		cLayer& pLayer = pLevel->GetLayer((UINT)i);

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

void cUIobj_Outliner::LoadGameObjectHierarchy(cUI_Tree* _pUI, cGameObject* _pGameObject)
{
	if (nullptr == _pGameObject)
		return;

	cUI_Tree* pTree = new cUI_Tree(_pGameObject->GetKey());

	pTree->SetDataPtr(tPtrData{ _pGameObject, });
	
	pTree->SetFuncCallback(eUI_MOUSE_STATUS::LBTN_DOUBLE_CLICKED,
		std::bind(&cUIobj_Outliner::CallbackLinkInspector, this, std::placeholders::_1));

	pTree->AddTreeFlag(ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen);

	_pUI->AddChildUI(pTree);

	size_t numChild = _pGameObject->GetNumChilds();
	if ((size_t)0 == numChild)
		return;

	const vector<cGameObject*>& vecChild = _pGameObject->GetvecChilds();
	for (size_t i = 0; i < numChild; ++i)
	{
		LoadGameObjectHierarchy(pTree, vecChild[i]);
	}
}

void cUIobj_Outliner::CallbackLinkInspector(tUIData _UIData)
{
	if (nullptr == _UIData.tData.ptr)
		return;

	m_pInspectorUI->SetTarget(static_cast<cGameObject*>(_UIData.tData.ptr));
}
