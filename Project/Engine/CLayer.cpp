#include "pch.h"
#include "CLayer.h"

#include "CGameObject.h"

CLayer::CLayer(int _iLayerIdx)
	: m_iLayerIdx(_iLayerIdx)
{
}

CLayer::~CLayer()
{
	Safe_Del_Vec(m_vecObject);
}

void CLayer::tick()
{
	size_t size = m_vecObject.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (true == m_vecObject[i]->IsMaster())
			m_vecObject[i]->tick();
	}
}

void CLayer::finaltick()
{
	size_t size = m_vecObject.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (true == m_vecObject[i]->IsMaster())
			m_vecObject[i]->finaltick();
	}
}

void CLayer::AddGameObject(CGameObject* _Object)
{
	if (nullptr == _Object)
		return;

	//자신의 오브젝트 리스트에 전체 계층구조를 추가한다.
	_Object->AddAllHierarchyObjects(m_iLayerIdx, m_vecObject);
}

void CLayer::RemoveGameObject(CGameObject* _Object)
{
	auto iter = m_vecObject.begin();
	auto iterEnd = m_vecObject.end();
	
	while (iter != iterEnd)
	{
		if (_Object == (*iter))
		{
			m_vecObject.erase(iter);
			return;
		}
		++iter;
	}
}

void CLayer::RemoveDestroyed()
{
	//Destroy 상태의 오브젝트를 vector에서 제거
	m_vecObject.erase(std::remove_if(m_vecObject.begin(), m_vecObject.end(),
		[](CGameObject* _Obj)->bool
		{
			return _Obj->GetDestroyed();
		}),
		m_vecObject.end());
}

