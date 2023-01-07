#include "pch.h"
#include "CLevel.h"

#include "CLayer.h"

#include "CGameObject.h"

CLevel::CLevel()
	: m_arrLayer{}
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i] = new CLayer;
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
	m_arrLayer[_iLayerIdx]->AddGameObject(_Object);
	_Object->init();
}
