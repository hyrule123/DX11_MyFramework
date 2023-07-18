#include "pch.h"
#include "cLayer.h"

#include "cEngine.h"

#include "cGameObject.h"

#include "cTransform.h"

cLayer::cLayer()
	: m_iLayerIdx(-1)
	, m_bYSort()
	, m_fPresetZDepth(FLT_MAX_NEGATIVE)
{
}

cLayer::~cLayer()
{
	//댕글링 포인터를 방지하기 위해 partition 먼저 진행후 Master 오브젝트만 제거
	auto iterEnd = std::partition(m_vecObject.begin(), m_vecObject.end(),
		[](cGameObject* _pObj)->bool
		{
			return _pObj->IsMaster();
		}
	);

	size_t vecEnd = iterEnd - m_vecObject.begin();
	for (size_t i = 0; i < vecEnd; ++i)
	{
		if (m_vecObject[i])
			delete m_vecObject[i];
	}
}

void cLayer::tick()
{
	size_t size = m_vecObject.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (true == m_vecObject[i]->IsMaster())
			m_vecObject[i]->tick();
	}
}

void cLayer::finaltick()
{
	size_t size = m_vecObject.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecObject[i]->Transform();

		if (true == m_vecObject[i]->IsMaster())
			m_vecObject[i]->finaltick();
	}
}

void cLayer::AddGameObject(cGameObject* _Object)
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

void cLayer::RemoveGameObject(cGameObject* _Object)
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

void cLayer::RemoveDestroyed()
{
	if (m_iLayerIdx == 6)
		int a = 0;

	//Destroy 상태의 오브젝트를 vector에서 제거
	m_vecObject.erase(std::remove_if(m_vecObject.begin(), m_vecObject.end(),
		[](cGameObject*& _Obj)->bool
		{
			bool isDestroyed = _Obj->IsDestroyed();

			if (isDestroyed && _Obj->IsMaster())
				delete _Obj;

			return isDestroyed;
		}),
		m_vecObject.end());
}


