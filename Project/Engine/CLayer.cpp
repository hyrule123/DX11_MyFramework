#include "pch.h"
#include "CLayer.h"

#include "CGameObject.h"

CLayer::CLayer(int _iLayerIdx)
	: m_iLayerIdx(_iLayerIdx)
{
}

CLayer::~CLayer()
{
	Safe_Del_List(m_listObject);
}

void CLayer::tick()
{
	for (auto& iter : m_listObject)
	{
		if(iter->IsMaster())
			iter->tick();
	}
}

void CLayer::finaltick()
{
	for (auto& iter : m_listObject)
	{
		if(iter->IsMaster())
			iter->finaltick();
	}
}

void CLayer::AddGameObject(CGameObject* _Object)
{
	if (nullptr == _Object)
		return;

	//자신의 오브젝트 리스트에 전체 계층구조를 추가한다.
	_Object->AddAllHierarchyObjects(m_iLayerIdx, m_listObject);
}

void CLayer::RemoveGameObject(CGameObject* _Object)
{
	auto iter = m_listObject.begin();
	auto iterEnd = m_listObject.end();
	
	while (iter != iterEnd)
	{
		if (_Object == (*iter))
		{
			m_listObject.erase(iter);
			return;
		}
		++iter;
	}

}

