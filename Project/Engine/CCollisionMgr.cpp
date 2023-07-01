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
#include "CCollider2D_Circle.h"

#include "struct.h"


CCollisionMgr::CCollisionMgr()
	: m_iNum2DGridX()
	, m_iNum2DGridY()
	, m_iNum2DGridTotalIndex()
	, m_arrFlagLayerInteraction{}
{
	RegisterCollisionFunc();


	AddLayerInteraction2D(1, 1);
	Create2DGrid(Vec2(-10000.f, -10000.f), Vec2(20000.f, 20000.f), (UINT)10, (UINT)10);
}

CCollisionMgr::~CCollisionMgr()
{

}

void CCollisionMgr::CalcSimpleCollGrid2D(__in CCollider2D* _pCol, __in Vec4 _vLBRTPos, __out vector<UINT>& _vecIdx)
{
	_vecIdx.clear();

	//그리드 기준의 좌표계로 변환한다.
	_vLBRTPos -= m_v4SpaceLBPos;
	//그리드 한 칸의 사이즈로 나눠 주면 점 한 칸의 인덱스 위치를 계산할 수 있음.
	_vLBRTPos *= m_v4GridSizeInv;

	//인덱스의 범위를 그리드 범위 안으로 한정
	enum { BeginX, BeginY, EndX, EndY };
	int iBeginXYEndXY[4] = {};
	iBeginXYEndXY[BeginX] = std::clamp((int)_vLBRTPos.x, 0, m_iNum2DGridX);
	iBeginXYEndXY[BeginY] = std::clamp((int)_vLBRTPos.y, 0, m_iNum2DGridY);
	iBeginXYEndXY[EndX] = std::clamp((int)_vLBRTPos.z, 0, m_iNum2DGridX);
	iBeginXYEndXY[EndY] = std::clamp((int)_vLBRTPos.w, 0, m_iNum2DGridY);

	for (int x = iBeginXYEndXY[BeginX]; x <= iBeginXYEndXY[EndX]; ++x)
	{
		for (int y = iBeginXYEndXY[BeginY]; y <= iBeginXYEndXY[EndY]; ++y)
		{
			UINT Idx = (UINT)(y * m_iNum2DGridX + x);
			
			//m_vec2DGrid[Idx].vecColl.push_back(_pCol);

			_vecIdx.push_back(Idx);
		}
	}
}


void CCollisionMgr::AddCollider2D(CCollider2D* _pCol, const vector<UINT>& _vecIdx)
{
	//그냥 순회 돌아주면서 그리드에 충돌검사해야될 충돌체 정점을 집어넣는다.
	size_t size = _vecIdx.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vec2DGrid[_vecIdx[i]].vecColl.push_back(_pCol);
	}
}



void CCollisionMgr::Create2DGrid(Vec2 _vWorldLB, Vec2 _vWorldSize, UINT _uiGridNumX, UINT _uiGridNumY)
{
	m_iNum2DGridX = _uiGridNumX;
	m_iNum2DGridY = _uiGridNumY;

	m_vSpaceLBPos = _vWorldLB;
	m_v4SpaceLBPos = Vec4(m_vSpaceLBPos.x, m_vSpaceLBPos.y, m_vSpaceLBPos.x, m_vSpaceLBPos.y);
	
	m_vSpaceSize = _vWorldSize;

	float GridSizeX = _vWorldSize.x / _uiGridNumX;
	float GridSizeY = _vWorldSize.y / _uiGridNumY;
	m_vGridSize = Vec2(GridSizeX, GridSizeY);


	//나눗셈은 곱셈보다 느리므로, 게산 최적화를 위해 나눗셈 값을 미리 구해 놓는다.
	m_vGridSizeInv.x = 1.f / GridSizeX;
	m_vGridSizeInv.y = 1.f / GridSizeY;
	m_v4GridSizeInv = Vec4(m_vGridSizeInv.x, m_vGridSizeInv.y, m_vGridSizeInv.x, m_vGridSizeInv.y);


	m_iNum2DGridTotalIndex = m_iNum2DGridX * m_iNum2DGridY;
	m_vec2DGrid.resize(m_iNum2DGridTotalIndex);
}

void CCollisionMgr::tick()
{
	//2D 충돌 검사
	for (int i = 0; i < m_iNum2DGridTotalIndex; ++i)
	{

		size_t size = m_vec2DGrid[i].vecColl.size();
		//충돌은 최소 2개가 있어야 진행 가능하므로 2사이즈가 2개 이하일 경우 비워주고 다음 벡터에 대해 충돌검사 진행
		if (size < 2)
		{
			m_vec2DGrid[i].vecColl.clear();
			continue;
		}
		
		//그리드 내부를 ID 순서대로 정렬한다.
		std::sort(m_vec2DGrid[i].vecColl.begin(), m_vec2DGrid[i].vecColl.end(),
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
				if (false == 
					(m_arrFlagLayerInteraction[m_vec2DGrid[i].vecColl[l]->GetLayerIndex()]
					&
					(UINT32)1 << m_vec2DGrid[i].vecColl[m]->GetLayerIndex())
					)
					continue;

				//만약 삭제 대기 상태일 경우 - CGameObject에서 tick과 finaltick을 돌리지 않음
				//->충돌체 정보가 공간분할 그리드에 등록되지 않음
				//->여기서 검사되지 않음
				//->혹시나 충돌중인 충돌체가 있었을 경우 아래의 충돌하지 않은 충돌체 제거 단계에서
				//제거되므로 문제가 발생하지 않음.

				//위에서 ID순 오름차순으로 정렬했으므로 ID를 굳이 비교할 필요가 없다.
				CollisionID ID = {}; 
				ID.LowID = m_vec2DGrid[i].vecColl[l]->GetID();
				ID.HighID = m_vec2DGrid[i].vecColl[m]->GetID();

				CCollider2D* pColA = m_vec2DGrid[i].vecColl[l];
				CCollider2D* pColB = m_vec2DGrid[i].vecColl[m];

				Vec2 HitPoint;

				//충돌
				if (true == CheckCollision2D(pColA, pColB, HitPoint))
				{

					auto iter = m_umapCollisionID.find(ID.FullID);
					if (iter == m_umapCollisionID.end())
					{
						pColA->BeginCollision(pColB, Vec3(HitPoint, 0.f));


						//충돌 정보에 충돌을 체크한 시간과 충돌체 주소를 넣어서 map에 삽입한다.
						tCollisionInfo Info = 
						{ CTimeMgr::GetInst()->GetCurCount(), 
							pColA, pColB };
						m_umapCollisionID.insert(make_pair(ID.FullID, Info));
					}
					else
					{
						pColA->OnCollision(pColB, Vec3(HitPoint, 0.f));

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
		m_vec2DGrid[i].vecColl.clear();
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

bool CCollisionMgr::CheckCollision2D(CCollider2D* _pCol_1, CCollider2D* _pCol_2, Vec2& _v2HitPoint)
{
	return m_arrFuncCheckCollision2D[(int)_pCol_1->GetColliderType()][(int)_pCol_2->GetColliderType()](_pCol_1, _pCol_2, _v2HitPoint);
}

bool CCollisionMgr::CheckCollision2D_Rect_Rect(CCollider2D* _pColRect_1, CCollider2D* _pColRect_2, Vec2& _v2HitPoint)
{
	const Vec4& LBRT_1 = _pColRect_1->GetSimpleCollider();
	const Vec4& LBRT_2 = _pColRect_2->GetSimpleCollider();

	enum LBRT{ L, B, R, T };
	if (LBRT_1[L] > LBRT_2[R])
		return false;
	else if (LBRT_1[R] < LBRT_2[L])
		return false;
	else if (LBRT_1[B] > LBRT_2[T])
		return false;
	else if (LBRT_1[T] < LBRT_2[B])
		return false;

	return true;
}

bool CCollisionMgr::CheckCollision2D_Rect_Circle(CCollider2D* _pColRect, CCollider2D* _pColCircle, Vec2& _v2HitPoint)
{
	enum LBRT { L, B, R, T };
	const Vec4& RectLBRT = _pColRect->GetSimpleCollider();
	const Vec2& CircleCenter = _pColCircle->GetCenterPos().XY();
	float CircleRadius = static_cast<CCollider2D_Circle*>(_pColCircle)->GetRadius();

	//원의 중심점이 사각형의 어느 부분에 있는지 계산한다.
	UINT idx = ComputeRelativePos_Rect_Point(RectLBRT, CircleCenter);
	
	//원의 중심점이 사각형 내부일 경우
	if (idx == 4u)
	{
		_v2HitPoint = CircleCenter;
		return true;
	}

	//우측으로 1비트 이동 + 1과 비트 and 연산 하면 2로 나눈 나머지 결과와 똑같음
	bool isOdd = (idx & 0x01);

	//1, 3, 5, 7일 경우 == 상하좌우에 위치할 경우
	if (isOdd)
	{
		float dist = 0.f;
		switch (idx)
		{
			//원의 중점이 사각형의 아래에 있을 경우(y < Bottom)
		case 1u:
			dist = fabsf(RectLBRT[B] - CircleCenter.y);
			break;

			//원의 중점이 사각형의 왼쪽에 있을 경우
		case 3u:
			dist = fabsf(RectLBRT[L] - CircleCenter.x);
			break;

			//원의 중점이 사각형의 오른쪽에 있을 경우
		case 5u:
			dist = fabsf(CircleCenter.x - RectLBRT[R]);
			break;

			//원의 중점이 사각형의 위에 있을 경우
		case 7u:
			dist = fabsf(CircleCenter.y - RectLBRT[T]);
			break;

		default:
			break;
		}

		//거리가 반지름보다 짧을 경우
		if (dist < CircleRadius)
		{
			ComputeHitPointByRectLBRT(_pColRect->GetSimpleCollider(), _pColCircle->GetSimpleCollider(), _v2HitPoint);
			return true;
		}
	}

	//0, 2, 6, 8(대각선 위치일 경우)
	else
	{
		//아닐 경우 사각형의 끝 꼭지점이 원의 범위 안인지 확인한다.
		enum LBRT { L, B, R, T };
		for (int i = 0; i < 4; ++i)
		{
			Vec2 RectVertex;
			switch (i)
			{
			case 0://LB
				RectVertex = Vec2(RectLBRT[L], RectLBRT[B]);
				break;
			case 1://RB
				RectVertex = Vec2(RectLBRT[R], RectLBRT[B]);
				break;
			case 2://RT
				RectVertex = Vec2(RectLBRT[R], RectLBRT[T]);
				break;
			case 3://LT
				RectVertex = Vec2(RectLBRT[L], RectLBRT[T]);
				break;
			}

			if (true == CheckCollision2D_Circle_Point_CollInfo(CircleCenter, CircleRadius, RectVertex))
			{
				ComputeHitPointByRectLBRT(_pColRect->GetSimpleCollider(), _pColCircle->GetSimpleCollider(), _v2HitPoint);
				return true;
			}
		}
	}

	return false;
}

bool CCollisionMgr::CheckCollision2D_Rect_OBB(CCollider2D* _pColRect, CCollider2D* _pColOBB, Vec2& _v2HitPoint)
{
	return false;
}

bool CCollisionMgr::CheckCollision2D_Rect_Point(CCollider2D* _pColRect, CCollider2D* _pColPoint, Vec2& _v2HitPoint)
{
	const Vec4& LBRT = _pColRect->GetSimpleCollider();
	const Vec2& Point = _pColPoint->GetCenterPos().XY();

	if (false == CheckCollision2D_Rect_Point_CollInfo(LBRT, Point))
		return false;

	_v2HitPoint = Point;
	return true;
}

bool CCollisionMgr::CheckCollision2D_Rect_Point_CollInfo(const Vec4& _v4LBRT, const Vec2& _v2Point)
{
	enum LBRT { L, B, R, T };
	if (_v2Point.x < _v4LBRT[L])
		return false;
	else if (_v2Point.x > _v4LBRT[R])
		return false;
	else if (_v2Point.y < _v4LBRT[B])
		return false;
	else if (_v2Point.y > _v4LBRT[T])
		return false;

	return true;
}

bool CCollisionMgr::CheckCollision2D_Circle_Circle(CCollider2D* _pColCircle_1, CCollider2D* _pColCircle_2, Vec2& _v2HitPoint)
{
	const Vec2& CenterPos_1 = _pColCircle_1->GetCenterPos().XY();
	const Vec2& CenterPos_2 = _pColCircle_2->GetCenterPos().XY();

	float dist = Vec2::DistanceSquared(CenterPos_1, CenterPos_2);

	float RadiusSum = static_cast<CCollider2D_Circle*>(_pColCircle_1)->GetRadius();
	RadiusSum += static_cast<CCollider2D_Circle*>(_pColCircle_2)->GetRadius();
	RadiusSum *= RadiusSum;

	if (dist > RadiusSum)
		return false;
	
	ComputeHitPointByRectLBRT(_pColCircle_1->GetSimpleCollider(), _pColCircle_2->GetSimpleCollider(), _v2HitPoint);

	return true;
}

bool CCollisionMgr::CheckCollision2D_Circle_OBB(CCollider2D* _pColCircle, CCollider2D* _pColOBB, Vec2& _v2HitPoint)
{
	return false;
}

bool CCollisionMgr::CheckCollision2D_Circle_Point(CCollider2D* _pColCircle, CCollider2D* _pColPoint, Vec2& _v2HitPoint)
{
	const Vec2& CenterPos = _pColCircle->GetCenterPos().XY();
	const Vec2& Point = _pColPoint->GetCenterPos().XY();

	float fRad = static_cast<CCollider2D_Circle*>(_pColCircle)->GetRadius();

	if (Vec2::DistanceSquared(CenterPos, Point) > fRad * fRad)
		return false;

	_v2HitPoint = Point;

	return true;
}

bool CCollisionMgr::CheckCollision2D_Circle_Point_CollInfo(const Vec2& _v2CircleCenterPos, float _fCircleRadius, const Vec2& _v2PointPos)
{
	float dist = Vec2::DistanceSquared(_v2CircleCenterPos, _v2PointPos);

	if (dist > _fCircleRadius * _fCircleRadius)
		return false;

	return true;
}

bool CCollisionMgr::CheckCollision2D_OBB_OBB(CCollider2D* _pColOBB2D_1, CCollider2D* _pColOBB2D_2, Vec2& _v2HitPoint)
{
	const tOBB2D& OBB_1 = static_cast<CCollider2D_OBB*>(_pColOBB2D_1)->GetOBBInfo();
	const tOBB2D& OBB_2 = static_cast<CCollider2D_OBB*>(_pColOBB2D_2)->GetOBBInfo();

	//Vec2의 사이즈 * 2를 계산한다. 최적화를 위해 처음 계산한 값을 계속 사용.
	static const size_t vec2_2size = sizeof(Vec2) * 2;

	//각 축의 정보를 순회하기 편하도록 가져온다.
	//OBB 구조체의 가장 첫 부분에는 X축, Y축에 대한 정보가 들어 있음. memcpy로 가져와주면 됨.
	Vec2 arrVec[4] = {};
	memcpy_s(&arrVec[0], vec2_2size, &OBB_1, vec2_2size);
	memcpy_s(&arrVec[2], vec2_2size, &OBB_2, vec2_2size);


	Vec2 VecMiddle = (_pColOBB2D_1->GetCenterPos() - _pColOBB2D_2->GetCenterPos()).XY();
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
	ComputeHitPointByRectLBRT(_pColOBB2D_1->GetSimpleCollider(), _pColOBB2D_2->GetSimpleCollider(), _v2HitPoint);

	return true;
}

bool CCollisionMgr::CheckCollision2D_OBB_Point(CCollider2D* _pColOBB2D, CCollider2D* _pColPoint, Vec2& _v2HitPoint)
{
	const tOBB2D& OBBInfo = static_cast<CCollider2D_OBB*>(_pColOBB2D)->GetOBBInfo();
	Vec2 vPointToOBBCenter = _pColPoint->GetCenterPos().XY();

	//점의 위치로부터 OBB의 중심 위치까지
	vPointToOBBCenter -= _pColOBB2D->GetCenterPos().XY();

	for (int i = 0; i < (int)eAXIS2D::END; ++i)
	{
		//사영의 대상이 될 축을 단위벡터로 전환
		Vec2 Norm = OBBInfo.m_v2Axis[i];
		Norm.Normalize();

		//각 축을 사영 대상 축에 사영하고, 0.5를 곱해서 반 길이로 줄인다.(중심부터의 거리이므로)
		float NormLen = 0.f;
		NormLen += fabs(Norm.Dot(OBBInfo.m_v2Axis[(int)eAXIS2D::X]));
		NormLen += fabs(Norm.Dot(OBBInfo.m_v2Axis[(int)eAXIS2D::Y]));
		NormLen *= 0.5f;

		//마찬가지로 점과 OBB의 중심점도 같은 축에 사영한다.
		float PointToOBBCenterProj = fabs(Norm.Dot(vPointToOBBCenter));
		
		//만약 XY축을 사영한 길이가 중점 사이의 거리보다 길 경우 충돌이 아니다.
		if (PointToOBBCenterProj > NormLen)
			return false;
	}


	_v2HitPoint = _pColPoint->GetCenterPos();

	return true;
}


bool CCollisionMgr::CheckCollision2D_Point_Point(CCollider2D* _pColPoint_1, CCollider2D* _pColPoint_2, Vec2& _v2HitPoint)
{
	//점 점은 무조건 return false
	return false;
}


UINT CCollisionMgr::ComputeRelativePos_Rect_Point(const Vec4& _v4RectLBRT, const Vec2& _v2Point)
{
	enum LBRT { L, B, R, T };
	UINT x =	_v2Point.x < _v4RectLBRT[L] ? 0 :
				_v2Point.x <= _v4RectLBRT[R] ? 1 : 2;

	UINT y =	_v2Point.y < _v4RectLBRT[B] ? 0 :
				_v2Point.y <= _v4RectLBRT[T] ? 1 : 2;

	return x + y * 3u;
}

void CCollisionMgr::ComputeHitPointByRectLBRT(const Vec4& _v4LBRT_1, const Vec4& _v4LBRT_2, Vec2& _v2HitPoint)
{
	enum LBRT { L, B, R, T };

	//모두 통과했을 경우 hit point 계산
	float Left = _v4LBRT_1[L] > _v4LBRT_2[L] ? _v4LBRT_1[L] : _v4LBRT_2[L];
	float Bottom = _v4LBRT_1[B] > _v4LBRT_2[B] ? _v4LBRT_1[B] : _v4LBRT_2[B];
	float Right = _v4LBRT_1[R] < _v4LBRT_2[R] ? _v4LBRT_1[R] : _v4LBRT_2[R];
	float Top = _v4LBRT_1[T] < _v4LBRT_2[T] ? _v4LBRT_1[T] : _v4LBRT_2[T];

	_v2HitPoint.x = (Left + Right) * 0.5f;
	_v2HitPoint.y = (Top + Bottom) * 0.5f;
}



void CCollisionMgr::RegisterCollisionFunc()
{
	//인덱스와 함수가 순서가 불일치할 경우 std::placeholders를 역순으로 배치할것

	//RECTANGLE
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::RECT][(int)eCOLLIDER_TYPE_2D::RECT]
		= std::bind(&CCollisionMgr::CheckCollision2D_Rect_Rect, this, 
			std::placeholders::_1, std::placeholders::_2, 
			std::placeholders::_3);

	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::RECT][(int)eCOLLIDER_TYPE_2D::CIRCLE]
		= std::bind(&CCollisionMgr::CheckCollision2D_Rect_Circle, this, 
			std::placeholders::_1, std::placeholders::_2,
			std::placeholders::_3);

	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::RECT][(int)eCOLLIDER_TYPE_2D::OBB]
		= std::bind(&CCollisionMgr::CheckCollision2D_Rect_OBB, this, 
			std::placeholders::_1, std::placeholders::_2, 
			std::placeholders::_3);

	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::RECT][(int)eCOLLIDER_TYPE_2D::POINT]
		= std::bind(&CCollisionMgr::CheckCollision2D_Rect_Point, this, 
			std::placeholders::_1, std::placeholders::_2,
			std::placeholders::_3);

	//CIRCLE
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::CIRCLE][(int)eCOLLIDER_TYPE_2D::RECT]
		= std::bind(&CCollisionMgr::CheckCollision2D_Rect_Circle, this, 
			std::placeholders::_2, std::placeholders::_1,
			std::placeholders::_3);

	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::CIRCLE][(int)eCOLLIDER_TYPE_2D::CIRCLE]
		= std::bind(&CCollisionMgr::CheckCollision2D_Circle_Circle, this, 
			std::placeholders::_1, std::placeholders::_2, 
			std::placeholders::_3);

	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::CIRCLE][(int)eCOLLIDER_TYPE_2D::OBB]
		= std::bind(&CCollisionMgr::CheckCollision2D_Circle_OBB, this, 
			std::placeholders::_1, std::placeholders::_2, 
			std::placeholders::_3);

	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::CIRCLE][(int)eCOLLIDER_TYPE_2D::POINT]
		= std::bind(&CCollisionMgr::CheckCollision2D_Circle_Point, this, 
			std::placeholders::_1, std::placeholders::_2, 
			std::placeholders::_3);

	//OBB
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::OBB][(int)eCOLLIDER_TYPE_2D::RECT]
		= std::bind(&CCollisionMgr::CheckCollision2D_Rect_OBB, this, 
			std::placeholders::_2, std::placeholders::_1, 
			std::placeholders::_3);

	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::OBB][(int)eCOLLIDER_TYPE_2D::CIRCLE]
		= std::bind(&CCollisionMgr::CheckCollision2D_Circle_OBB, this, 
			std::placeholders::_2, std::placeholders::_1, 
			std::placeholders::_3);

	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::OBB][(int)eCOLLIDER_TYPE_2D::OBB]
		= std::bind(&CCollisionMgr::CheckCollision2D_OBB_OBB, this, 
			std::placeholders::_1, std::placeholders::_2, 
			std::placeholders::_3);

	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::OBB][(int)eCOLLIDER_TYPE_2D::POINT]
		= std::bind(&CCollisionMgr::CheckCollision2D_OBB_Point, this, 
			std::placeholders::_1, std::placeholders::_2, 
			std::placeholders::_3);


	//POINT
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::POINT][(int)eCOLLIDER_TYPE_2D::RECT]
		= std::bind(&CCollisionMgr::CheckCollision2D_Rect_Point, this, 
			std::placeholders::_2, std::placeholders::_1, 
			std::placeholders::_3);

	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::POINT][(int)eCOLLIDER_TYPE_2D::CIRCLE]
		= std::bind(&CCollisionMgr::CheckCollision2D_Circle_Point, this, 
			std::placeholders::_2, std::placeholders::_1, 
			std::placeholders::_3);
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::POINT][(int)eCOLLIDER_TYPE_2D::OBB]
		= std::bind(&CCollisionMgr::CheckCollision2D_OBB_Point, this, 
			std::placeholders::_2, std::placeholders::_1, 
			std::placeholders::_3);

	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::POINT][(int)eCOLLIDER_TYPE_2D::POINT]
		= std::bind(&CCollisionMgr::CheckCollision2D_Point_Point, this, 
			std::placeholders::_1, std::placeholders::_2, 
			std::placeholders::_3);
}
