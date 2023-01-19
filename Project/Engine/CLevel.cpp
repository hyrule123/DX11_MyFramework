#include "pch.h"
#include "CLevel.h"

#include "CLayer.h"

#include "CGameObject.h"

CLevel::CLevel()
	: m_arrLayer{}
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i] = new CLayer(i);
	}
}

CLevel::~CLevel()
{
	Safe_Del_Array(m_arrLayer);
}


void CLevel::tick()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->tick();
	}
}

void CLevel::finaltick()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->finaltick();
	}
}


void CLevel::AddGameObject(CGameObject* _Object, int _iLayerIdx)
{
	assert(nullptr != _Object || 0 < _iLayerIdx);

	m_arrLayer[_iLayerIdx]->AddGameObject(_Object);
	_Object->init();
}

void CLevel::RemoveDestroyed()
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->RemoveDestroyed();
	}
}

void CLevel::SetLayerName(int _iLayer, const wstring& _sLayerName)
{
	assert(0 <= _iLayer && _iLayer < MAX_LAYER);

	m_arrLayer[_iLayer]->SetName(_sLayerName);
}

const wstring& CLevel::GetLayerName(int _iLayer)
{
	assert(0 <= _iLayer && _iLayer < MAX_LAYER);

	return m_arrLayer[_iLayer]->GetName();
}

int CLevel::GetLayerIdxByName(const wstring& _sLayerName)
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		if (_sLayerName == m_arrLayer[i]->GetName())
			return i;
	}

	//이름으로 못찾았을 경우 -1을 리턴
	return -1;
}
