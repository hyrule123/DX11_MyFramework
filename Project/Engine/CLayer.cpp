#include "pch.h"
#include "CLayer.h"

#include "CGameObject.h"

CLayer::CLayer()
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
		m_vecObject[i]->tick();
	}
}

void CLayer::finaltick()
{
	size_t size = m_vecObject.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecObject[i]->finaltick();
	}
}

void CLayer::render()
{
	size_t size = m_vecObject.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecObject[i]->render();
	}
}