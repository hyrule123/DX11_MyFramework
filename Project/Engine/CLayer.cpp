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

	//�ڽ��� ������Ʈ ����Ʈ�� ��ü ���������� �߰��Ѵ�.
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

