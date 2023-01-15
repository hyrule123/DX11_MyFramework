#include "pch.h"
#include "CCollisionMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"


CCollisionMgr::CCollisionMgr()
	: m_SpatialPartition2D(eDIMENSION_2D)
	, m_SpatialPartition3D(eDIMENSION_3D)
{
}

CCollisionMgr::~CCollisionMgr()
{
}


void CCollisionMgr::AddLayerInteract(UINT _iLowLayer, UINT _iHighLayer)
{
	UINT Low = _iLowLayer;
	UINT High = _iHighLayer;
	if (Low > High)
	{
		UINT temp = Low;
		Low = High;
		High = temp;
	}

	//왼쪽으로 올라가는 계단 형태(행렬의 우측 상단만 사용)
	//ex)1번 레이어의 0번째 = 0번 레이어의 1번째
	//
	m_flagLayerCollision[Low] |= 1 << High;
}



void CCollisionMgr::AddCollider2D(CCollider2D* _pCol, const Vec2& _vLBPos, const Vec2& _vColSize)
{
	m_SpatialPartition2D->ComputeGridArea(_pCol, Vec3(_vLBPos, 0.f), Vec3(_vColSize, 0.f));
}


void CCollisionMgr::CollisionBtwLayer(int _iLayerNumLow, int _iLayerNumHigh)
{
	for()




}

void CCollisionMgr::CollisionBtwObject(CGameObject* _pLeft, CGameObject* _pRight)
{
}

void CCollisionMgr::CollisionBtwCollider(CCollider2D* _pLeft, CCollider2D* _pRight)
{
}

void CCollisionMgr::init()
{
}

void CCollisionMgr::tick()
{
	m_umapColID.clear();

	CLevel* pLevel = CLevelMgr::GetInst()->GetCurLevel();
	for (UINT iRow = 0; iRow < MAX_LAYER; ++iRow)
	{
		for (UINT iCol = iRow; iCol < MAX_LAYER; ++iCol)
		{
			if (false == (m_flagLayerCollision[iRow] & (1 << iCol)))
				continue;

			//iCol = High, iRow = Low
			CollisionBtwLayer(iRow, iCol);
		}
	}
}