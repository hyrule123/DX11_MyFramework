#include "pch.h"
#include "CLayer.h"

#include "CGameObject.h"

CLayer::CLayer()
	: m_iLayerIdx(-1)
{
}

CLayer::~CLayer()
{
	size_t size = m_vecObject.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (nullptr == m_vecObject[i]->GetParent())
			DESTRUCTOR_DELETE(m_vecObject[i]);
	}
}

void CLayer::tick()
{
	assert(0 <= m_iLayerIdx && m_iLayerIdx < MAX_LAYER);

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
	assert(nullptr != _Object);

	m_vecObject.push_back(_Object);
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
			bool isDestroyed = _Obj->IsDestroyed();

			if (isDestroyed)
				delete _Obj;

			return isDestroyed;
		}),
		m_vecObject.end());
}

