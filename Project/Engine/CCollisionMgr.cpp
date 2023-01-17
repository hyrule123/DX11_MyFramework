#include "pch.h"
#include "CCollisionMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"

#include "CSpatialPartition2D.h"
#include "CSpatialPartition3D.h"


CCollisionMgr::CCollisionMgr()
	: m_SpatialPartition2D()
	, m_SpatialPartition3D()
{
}

CCollisionMgr::~CCollisionMgr()
{
	if (nullptr != m_SpatialPartition2D) delete m_SpatialPartition2D;
	if (nullptr != m_SpatialPartition3D) delete m_SpatialPartition3D;
}



void CCollisionMgr::AddCollider2D(CCollider2D* _pCol)
{
	m_SpatialPartition2D->AddCollider(_pCol);
}

void CCollisionMgr::AddLayerInteraction2D(int _iLayer1, int _iLayer2)
{
	m_SpatialPartition2D->AddLayerInteract((UINT32)_iLayer1, (UINT32)_iLayer2);
}


void CCollisionMgr::init()
{
	m_SpatialPartition2D = new CSpatialPartition2D(400.f, 3);
	m_SpatialPartition2D->AddLayerInteract((UINT32)1, (UINT32)1);
}

void CCollisionMgr::tick()
{
	//공간 분할 클래스에서 충돌 검사를 실시
	if (nullptr != m_SpatialPartition2D)
		m_SpatialPartition2D->tick();

	if (nullptr != m_SpatialPartition3D)
		m_SpatialPartition3D->tick();
}