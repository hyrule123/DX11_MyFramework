#include "pch.h"
#include "CLayer.h"

#include "CGameObject.h"

CLayer::CLayer()
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
		iter->tick();
	}
}

void CLayer::finaltick()
{
	for (auto& iter : m_listObject)
	{
		iter->finaltick();
	}
}

