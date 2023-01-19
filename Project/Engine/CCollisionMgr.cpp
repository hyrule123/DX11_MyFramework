#include "pch.h"
#include "CCollisionMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"

#include "CCollider2D.h"


CCollisionMgr::CCollisionMgr()
{
	AddLayerInteraction2D(1, 1);
	Create2DGrid(Vec2(-10000.f, -10000.f), Vec2(20000.f, 20000.f), (UINT)40, (UINT)40);
}

CCollisionMgr::~CCollisionMgr()
{

}


void CCollisionMgr::AddCollider2D(CCollider2D* _pCol, tRectLBRT _AABBInfo)
{
	enum LBRT
	{
		L,
		B,
		R,
		T
	};

	_AABBInfo -= m_v4_2DSpaceLB;
	_AABBInfo *= m_v4_2DGridSizeInv;

	int GridIndex[4] = {(int)_AABBInfo.x, (int)_AABBInfo.y, (int)_AABBInfo.z, (int)_AABBInfo.w};
	
	GridIndex[L] = max(0, GridIndex[L]);
	GridIndex[B] = max(0, GridIndex[B]);
	GridIndex[R] = min((int)m_uiNum2DGridX, GridIndex[R]);
	GridIndex[T] = min((int)m_uiNum2DGridY, GridIndex[T]);
	
	for (int x = GridIndex[L]; x <= GridIndex[R]; ++x)
	{
		for (int y = GridIndex[B]; y <= GridIndex[T]; ++y)
		{
			m_vec2DGrid[y * m_uiNum2DGridX + x].vecColliderInGrid[_pCol->GetLayerIndex()].push_back(_pCol);
		}
	}
}



void CCollisionMgr::Create2DGrid(Vec2 _vWorldLB, Vec2 _vWorldSize, UINT _uiGridNumX, UINT _uiGridNumY)
{
	m_uiNum2DGridX = _uiGridNumX;
	m_uiNum2DGridY = _uiGridNumY;

	m_v2DSpaceLB = _vWorldLB;
	m_v4_2DSpaceLB = Vec4(_vWorldLB.x, _vWorldLB.y, _vWorldLB.x, _vWorldLB.y);
	
	m_v2DSpaceSize = _vWorldSize;

	float GridSizeX = _vWorldSize.x / _uiGridNumX;
	float GridSizeY = _vWorldSize.y / _uiGridNumY;
	m_v2DGridSize = Vec2(GridSizeX, GridSizeY);

	GridSizeX = 1.f / GridSizeX;
	GridSizeY = 1.f / GridSizeY;
	m_v4_2DGridSizeInv = Vec4(GridSizeX, GridSizeY, GridSizeX, GridSizeY);

	m_uiNum2DGridTotalIndex = m_uiNum2DGridX * m_uiNum2DGridY;
	m_vec2DGrid.resize(m_uiNum2DGridTotalIndex);
}

void CCollisionMgr::tick()
{
	for (auto& iter : m_umapCollisionID)
	{
		iter.second.bCurrent = false;
	}


	for (UINT i = 0; i < m_uiNum2DGridTotalIndex; ++i)
	{
		for (int j = 0; j < MAX_LAYER; ++j)
		{
			for (int k = j; k < MAX_LAYER; ++k)
			{
				if (false == (m_arrFlagLayerInteraction[j] & ((UINT32)1 << k)))
					continue;

				size_t lsize = m_vec2DGrid[i].vecColliderInGrid[j].size();
				for (int l = 0; l < lsize; ++l)
				{
					size_t msize = m_vec2DGrid[i].vecColliderInGrid[k].size();
					for (int m = 0; m < msize; ++m)
					{
						
						if (m_vec2DGrid[i].vecColliderInGrid[j][l] == m_vec2DGrid[i].vecColliderInGrid[k][m])
							continue;

						//충돌
						if (true == m_vec2DGrid[i].vecColliderInGrid[j][l]->CheckCollision(m_vec2DGrid[i].vecColliderInGrid[k][m]))
						{
							CollisionID ID = {};
							ID.LowID = m_vec2DGrid[i].vecColliderInGrid[j][l]->GetID();
							ID.HighID = m_vec2DGrid[i].vecColliderInGrid[k][m]->GetID();
							if (ID.HighID > ID.LowID)
							{
								UINT32 Temp = ID.LowID;
								ID.LowID = ID.HighID;
								ID.HighID = Temp;
							}

							auto iter = m_umapCollisionID.find(ID.FullID);
							if (iter == m_umapCollisionID.end())
							{
								m_vec2DGrid[i].vecColliderInGrid[j][l]->BeginCollision(m_vec2DGrid[i].vecColliderInGrid[k][m]);


								tCollisionInfo Info = {};
								Info.bCurrent = true;
								Info.pColliderA = m_vec2DGrid[i].vecColliderInGrid[j][l];
								Info.pColliderB = m_vec2DGrid[i].vecColliderInGrid[k][m];
								m_umapCollisionID.insert(make_pair(ID.FullID, Info));
							}
							else
							{
								m_vec2DGrid[i].vecColliderInGrid[j][l]->OnCollision(m_vec2DGrid[i].vecColliderInGrid[k][m]);

								iter->second.bCurrent = true;
							}

						}
						//비충돌
						else
						{
							CollisionID ID = {};
							ID.LowID = m_vec2DGrid[i].vecColliderInGrid[j][l]->GetID();
							ID.HighID = m_vec2DGrid[i].vecColliderInGrid[k][m]->GetID();
							if (ID.HighID > ID.LowID)
							{
								UINT32 Temp = ID.LowID;
								ID.LowID = ID.HighID;
								ID.HighID = Temp;
							}

							auto iter = m_umapCollisionID.find(ID.FullID);
							if (iter != m_umapCollisionID.end())
							{
								m_vec2DGrid[i].vecColliderInGrid[j][l]->EndCollision(m_vec2DGrid[i].vecColliderInGrid[k][m]);

								m_umapCollisionID.erase(iter);
							}

						}
					}
				}
					
			}

			//확인한 레이어들은 clear 처리
			m_vec2DGrid[i].vecColliderInGrid[j].clear();
		}
	} 
}

