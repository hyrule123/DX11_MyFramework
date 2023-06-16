#include "pch.h"
#include "CLayer.h"

#include "CGameObject.h"

#include "CTransform.h"

CLayer::CLayer()
	: m_iLayerIdx(-1)
	, m_fDepthPreset(DEPTH_PRESET_MAX - 100.f)
	, m_bEnableYsorting()
{
}

CLayer::~CLayer()
{
	size_t size = m_vecObject.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (nullptr == m_vecObject[i]->GetParent())
			SAFE_DELETE(m_vecObject[i]);
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
		m_vecObject[i]->Transform();

		if (true == m_vecObject[i]->IsMaster())
			m_vecObject[i]->finaltick();
	}
}

void CLayer::AddGameObject(CGameObject* _Object)
{
	assert(nullptr != _Object);

	m_vecObject.push_back(_Object);

	//깊이 프리셋값이 설정되어 있을 경우 해당 값으로 깊이값을 설정해준다.
	if (m_fDepthPreset > DEPTH_PRESET_MAX)
	{
		_Object->Transform()->SetRelativePosZ(m_fDepthPreset);
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

bool CLayer::Y_Sort(CGameObject* _pObj_L, CGameObject* _pObj_R)
{


	return false;
}

