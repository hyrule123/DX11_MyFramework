#include "pch.h"
#include "CCollisionMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"

#include "CCollider2D.h"

#include "CTimeMgr.h"

CCollisionMgr::CCollisionMgr()
{
	AddLayerInteraction2D(1, 1);
	Create2DGrid(Vec2(-10000.f, -10000.f), Vec2(20000.f, 20000.f), (UINT)10, (UINT)10);
}

CCollisionMgr::~CCollisionMgr()
{

}


void CCollisionMgr::AddCollider2D(CCollider2D* _pCol, tRectInfo _AABBInfo)
{
	enum LBRT{ L, B, R, T };

	_AABBInfo.LBRT -= m_v4_2DSpaceLB;
	_AABBInfo.LBRT *= m_v4_2DGridSizeInv;

	int GridIndex[4] = {(int)_AABBInfo.LBRT.x, (int)_AABBInfo.LBRT.y, (int)_AABBInfo.LBRT.z, (int)_AABBInfo.LBRT.w};
	
	GridIndex[L] = max(0, GridIndex[L]);
	GridIndex[B] = max(0, GridIndex[B]);
	GridIndex[R] = min((int)m_uiNum2DGridX, GridIndex[R]);
	GridIndex[T] = min((int)m_uiNum2DGridY, GridIndex[T]);
	
	for (int x = GridIndex[L]; x <= GridIndex[R]; ++x)
	{
		for (int y = GridIndex[B]; y <= GridIndex[T]; ++y)
		{
			int idx = y * m_uiNum2DGridX + x;
			m_vec2DGrid[y * m_uiNum2DGridX + x].vecColliderInGrid.push_back(_pCol);
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
	for (UINT i = 0; i < m_uiNum2DGridTotalIndex; ++i)
	{
		size_t size = m_vec2DGrid[i].vecColliderInGrid.size();
		//충돌은 최소 2개가 있어야 진행 가능하므로 2사이즈가 2개 이하일 경우 비워주고 다음 벡터에 대해 충돌검사 진행
		if (size < 2)
		{
			m_vec2DGrid[i].vecColliderInGrid.clear();
			continue;
		}
			

		std::sort(m_vec2DGrid[i].vecColliderInGrid.begin(), m_vec2DGrid[i].vecColliderInGrid.end(),
			[](CCollider2D* _pColA, CCollider2D* _pColB)->bool
			{
				return _pColA->GetID() < _pColB->GetID();
			}
		);

		for (int l = 0; l < size; ++l)
		{
			for (int m = l + 1; m < size; ++m)
			{

				if (false == (m_arrFlagLayerInteraction[m_vec2DGrid[i].vecColliderInGrid[l]->GetLayerIndex()]
					&
					(UINT32)1 << m_vec2DGrid[i].vecColliderInGrid[m]->GetLayerIndex())
					)
					continue;

				//위에서 ID순 오름차순으로 정렬했으므로 ID를 굳이 비교할 필요가 없다.
				CollisionID ID = {}; 
				ID.LowID = m_vec2DGrid[i].vecColliderInGrid[l]->GetID();
				ID.HighID = m_vec2DGrid[i].vecColliderInGrid[m]->GetID();

				//충돌
				if (true == m_vec2DGrid[i].vecColliderInGrid[l]->CheckCollision(m_vec2DGrid[i].vecColliderInGrid[m]))
				{

					auto iter = m_umapCollisionID.find(ID.FullID);
					if (iter == m_umapCollisionID.end())
					{
						m_vec2DGrid[i].vecColliderInGrid[l]->BeginCollision(m_vec2DGrid[i].vecColliderInGrid[m]);


						//충돌 정보에 충돌을 체크한 시간과 충돌체 주소를 넣어서 map에 삽입한다.
						tCollisionInfo Info = 
						{ CTimeMgr::GetInst()->GetCurCount(), 
							m_vec2DGrid[i].vecColliderInGrid[l], m_vec2DGrid[i].vecColliderInGrid[m]};
						m_umapCollisionID.insert(make_pair(ID.FullID, Info));
					}
					else
					{
						m_vec2DGrid[i].vecColliderInGrid[l]->OnCollision(m_vec2DGrid[i].vecColliderInGrid[m]);

						//충돌 중이라면(map에 충돌 정보가 들어 있다면) 체크한 시간만 갱신한다.
						iter->second.llCheckedCount = CTimeMgr::GetInst()->GetCurCount();
					}

				}
				//비충돌
				else
				{
					auto iter = m_umapCollisionID.find(ID.FullID);
					if (iter != m_umapCollisionID.end())
					{
						//충돌이 끝났는데 ID가 남아있다면 충돌 끝 함수를 호출하고 충돌 정보를 제거한다.
						m_vec2DGrid[i].vecColliderInGrid[l]->EndCollision(m_vec2DGrid[i].vecColliderInGrid[m]);

						m_umapCollisionID.erase(iter);
					}

				}
			}
		}

		//그리드 내부의 충돌체 정보를 비워준다.
		m_vec2DGrid[i].vecColliderInGrid.clear();
	}

	//이번 타임에 충돌확인되지 않은 충돌체들에 대해서 충돌 제거 처리
	//이번 틱의 시간값을 우선 받아온다.
	LONGLONG CurTime = CTimeMgr::GetInst()->GetCurCount();
	auto iter = m_umapCollisionID.begin();
	const auto& iterEnd = m_umapCollisionID.end();
	while (iter != iterEnd)
	{
		//기록된 시간이 이번 tick의 시간이 아닐 경우 충돌 해체 처리하고 제거한다.
		if (CurTime != iter->second.llCheckedCount)
		{
			iter->second.pColliderA->EndCollision(iter->second.pColliderB);
			iter = m_umapCollisionID.erase(iter);
		}
		else
			++iter;
	}

}