#include "pch.h"
#include "CLayer.h"

#include "CEngine.h"

#include "CGameObject.h"

#include "CTransform.h"

CLayer::CLayer()
	: m_iLayerIdx(-1)
	, m_bYSort()
	, m_fPresetZDepth(FLT_MAX_NEGATIVE)
{
}

CLayer::~CLayer()
{
	size_t size = m_vecObject.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (m_vecObject[i]->IsMaster())
			SAFE_DELETE(m_vecObject[i]);
	}
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
		m_vecObject[i]->Transform();

		if (true == m_vecObject[i]->IsMaster())
			m_vecObject[i]->finaltick();
	}
}

void CLayer::AddGameObject(CGameObject* _Object)
{
	assert(nullptr != _Object);

	m_vecObject.push_back(_Object);

	_Object->SetLayer(m_iLayerIdx);
		
	//프리셋 깊이가 설정되어 있을 경우
	if (FLT_MAX_NEGATIVE < m_fPresetZDepth)
	{
		if (_Object->IsMaster())
			_Object->Transform().SetRelativePosZ(m_fPresetZDepth);
		else
			_Object->Transform().SetRelativePosZ(-1.f);
	}
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
	if (m_iLayerIdx == 6)
		int a = 0;

	//Destroy 상태의 오브젝트를 vector에서 제거
	m_vecObject.erase(std::remove_if(m_vecObject.begin(), m_vecObject.end(),
		[](CGameObject*& _Obj)->bool
		{
			bool isDestroyed = _Obj->IsDestroyed();

			if (isDestroyed && _Obj->IsMaster())
				delete _Obj;

			return isDestroyed;
		}),
		m_vecObject.end());
}


