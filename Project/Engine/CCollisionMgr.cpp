#include "pch.h"
#include "CCollisionMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"

#include "CCollider2D.h"

#include "CTimeMgr.h"


#include "CCollider2D_OBB.h"
#include "CCollider2D_Point.h"

CCollisionMgr::CCollisionMgr()
{
	AddLayerInteraction2D(1, 1);
	Create2DGrid(Vec2(-10000.f, -10000.f), Vec2(20000.f, 20000.f), (UINT)10, (UINT)10);
}

CCollisionMgr::~CCollisionMgr()
{

}


void CCollisionMgr::AddCollider2D(CCollider2D* _pCol)
{
	enum LBRT{ L, B, R, T };

	//AABB 사각형 정보를 받아온다.
	tRectInfo AABBInfo = _pCol->GetSpatialPartitionInfo();

	//그리드 기준의 좌표계로 변환한다.
	AABBInfo.LBRT -= m_v4_2DSpaceLB;

	//그리드 한 칸의 사이즈로 상하좌우 인덱스값을 구한다.
	AABBInfo.LBRT *= m_v4_2DGridSizeInv;

	int GridIndex[4] = {(int)AABBInfo.LBRT.x, (int)AABBInfo.LBRT.y, (int)AABBInfo.LBRT.z, (int)AABBInfo.LBRT.w};

	//에러 방지용 minmax 확인
	GridIndex[L] = max(0, GridIndex[L]);
	GridIndex[B] = max(0, GridIndex[B]);
	GridIndex[R] = min((int)m_uiNum2DGridX, GridIndex[R]);
	GridIndex[T] = min((int)m_uiNum2DGridY, GridIndex[T]);


	//1차원 인덱스를 계산한다. 추가는 나중에 진행
	vector<UINT> vecCurGridIdx;
	for (int x = GridIndex[L]; x <= GridIndex[R]; ++x)
	{
		for (int y = GridIndex[B]; y <= GridIndex[T]; ++y)
		{
			vecCurGridIdx.push_back((UINT)(y* m_uiNum2DGridX + x));
		}
	}

	//이전 그리드 인덱스 목록을 받아온다.
	vector<UINT>& vecPrevGridIdx = _pCol->GetvecGridIdx();

	vector<UINT> vecAddDelete;

	//트랜스폼 변했을때만 여기서 새로 계산
	//아니면 그냥 가지고 있던 인덱스 번호로 등록

	//새로 들어간 그리드에 추가
	auto iter = std::set_difference(
		vecCurGridIdx.begin(), vecCurGridIdx.end(), 
		vecPrevGridIdx.begin(), vecPrevGridIdx.end(),
		vecAddDelete.begin());
	vecAddDelete.resize(iter - vecAddDelete.begin());
	size_t size = vecAddDelete.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vec2DGrid[vecAddDelete[i]].vecColliderInGrid.push_back(_pCol);
	}
	

	iter = std::set_difference(
		vecPrevGridIdx.begin(), vecPrevGridIdx.end(),
		vecCurGridIdx.begin(), vecCurGridIdx.end(),
		vecAddDelete.begin()
	);
	size_t size = vecAddDelete.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vec2DGrid[vecAddDelete[i]].vecColliderInGrid.erase
		(
			std::remove_if
			(
				m_vec2DGrid[vecAddDelete[i]].vecColliderInGrid.begin(), m_vec2DGrid[vecAddDelete[i]].vecColliderInGrid.end(),
				[=](CCollider2D* Col)->bool
				{
					return (Col == _pCol);
				}
			),
			m_vec2DGrid[vecAddDelete[i]].vecColliderInGrid.end()
					);
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

	//나눗셈은 곱셈보다 느리므로, 게산 최적화를 위해 나눗셈 값을 미리 구해 놓는다.
	GridSizeX = 1.f / GridSizeX;
	GridSizeY = 1.f / GridSizeY;
	m_v4_2DGridSizeInv = Vec4(GridSizeX, GridSizeY, GridSizeX, GridSizeY);

	m_uiNum2DGridTotalIndex = m_uiNum2DGridX * m_uiNum2DGridY;
	//m_vec2DGrid.resize(m_uiNum2DGridTotalIndex);
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
			
		//그리드 내부를 ID 순서대로 정렬한다.
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
				//두 레이어가 충돌하도록 설정되어 있는지 여부를 검사한다. 충돌하지 않는 레이어일 경우 충돌 검사 X
				if (false == (m_arrFlagLayerInteraction[m_vec2DGrid[i].vecColliderInGrid[l]->GetLayerIndex()]
					&
					(UINT32)1 << m_vec2DGrid[i].vecColliderInGrid[m]->GetLayerIndex())
					)
					continue;

				//위에서 ID순 오름차순으로 정렬했으므로 ID를 굳이 비교할 필요가 없다.
				CollisionID ID = {}; 
				ID.LowID = m_vec2DGrid[i].vecColliderInGrid[l]->GetID();
				ID.HighID = m_vec2DGrid[i].vecColliderInGrid[m]->GetID();

				CCollider2D* pColA = m_vec2DGrid[i].vecColliderInGrid[l];
				CCollider2D* pColB = m_vec2DGrid[i].vecColliderInGrid[m];

				//충돌
				if (true == CheckCollision2D(pColA, pColB))
				{

					auto iter = m_umapCollisionID.find(ID.FullID);
					if (iter == m_umapCollisionID.end())
					{
						pColA->BeginCollision(pColB);


						//충돌 정보에 충돌을 체크한 시간과 충돌체 주소를 넣어서 map에 삽입한다.
						tCollisionInfo Info = 
						{ CTimeMgr::GetInst()->GetCurCount(), 
							pColA, pColB };
						m_umapCollisionID.insert(make_pair(ID.FullID, Info));
					}
					else
					{
						pColA->OnCollision(pColB);

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
						pColA->EndCollision(pColB);

						m_umapCollisionID.erase(iter);
					}

				}
			}
		}

		//그리드 내부의 충돌체 정보를 비워준다.
		m_vec2DGrid[i].vecColliderInGrid.clear();
	}

	//이번 타임에 충돌확인되지 않은 충돌체들에 대해서 충돌 제거 처리
	//이번 틱에 기록된 진동값을 받아온다.
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

bool CCollisionMgr::CheckCollision2D(CCollider2D* _pCol_1, CCollider2D* _pCol_2)
{
	return false;
}

bool CCollisionMgr::CheckCollision2D_Rect_Rect(CCollider2D_Rect* _pColRect_1, CCollider2D_Rect* _pColRect_2)
{
	return false;
}

bool CCollisionMgr::CheckCollision2D_Circle_Circle(CCollider2D_Circle* _pColCircle_1, CCollider2D_Circle* _pColCircle_2)
{
	return false;
}

bool CCollisionMgr::CheckCollision2D_OBB2D_OBB2D(CCollider2D_OBB* _pColOBB2D_1, CCollider2D_OBB* _pColOBB2D_2)
{
	const tOBB2D& OBB_1 = _pColOBB2D_1->GetColliderInfo();
	const tOBB2D& OBB_2 = _pColOBB2D_2->GetColliderInfo();

	//Vec2의 사이즈 * 2를 계산한다. 최적화를 위해 처음 계산한 값을 계속 사용.
	static const size_t vec2_2size = sizeof(Vec2) * 2;

	//각 축의 정보를 순회하기 편하도록 가져온다.
	Vec2 arrVec[4] = {};
	memcpy_s(&arrVec[0], vec2_2size, &OBB_1, vec2_2size);
	memcpy_s(&arrVec[2], vec2_2size, &OBB_2, vec2_2size);


	Vec2 VecMiddle = OBB_1.m_vMiddle - OBB_2.m_vMiddle;
	for (int i = 0; i < 4; ++i)
	{
		Vec2 norm = arrVec[i];
		norm.Normalize();

		float fProjSum = 0.f;
		for (int j = 0; j < 4; ++j)
		{
			fProjSum += fabsf(norm.Dot(arrVec[j]));
		}

		//한 변의 길이(vSize)가 곱해졌으므로 0.5를 곱해줘야 실제 축의 길이가 됨
		fProjSum *= 0.5f;

		float fCenterProj = fabsf(norm.Dot(VecMiddle));

		if (fProjSum < fCenterProj)
			return false;
	}
	

	//위 검사를 통과하면 HitPoint를 계산해서 각 충돌체의 충돌시작함수를 호출한다.



	return true;
}

bool CCollisionMgr::CheckCollision2D_OBB2D_Point(CCollider2D_OBB* _pColOBB2D, CCollider2D_Point* _pColPoint)
{
	return false;
}
