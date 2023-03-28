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

void CCollisionMgr::CalcAndAddCollider2D(__in CCollider2D* _pCol, __in Vec4 _vLBRTPos, __out vector<UINT>& _vecIdx)
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
			
			m_vec2DGrid[Idx].vecColl.push_back(_pCol);

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
				//->여기서 감사되지 않음
				//->혹시나 충돌중인 충돌체가 있었을 경우 아래의 충돌하지 않은 충돌체 제거 단계에서
				//제거되므로 문제가 발생하지 않음.

				//위에서 ID순 오름차순으로 정렬했으므로 ID를 굳이 비교할 필요가 없다.
				CollisionID ID = {}; 
				ID.LowID = m_vec2DGrid[i].vecColl[l]->GetID();
				ID.HighID = m_vec2DGrid[i].vecColl[m]->GetID();

				CCollider2D* pColA = m_vec2DGrid[i].vecColl[l];
				CCollider2D* pColB = m_vec2DGrid[i].vecColl[m];

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

bool CCollisionMgr::CheckCollision2D(CCollider2D* _pCol_1, CCollider2D* _pCol_2)
{
	return m_arrFuncCheckCollision2D[(int)_pCol_1->GetColliderType()][(int)_pCol_2->GetColliderType()](_pCol_1, _pCol_2);
}

bool CCollisionMgr::CheckCollision2D_Rect_Rect(CCollider2D* _pColRect_1, CCollider2D* _pColRect_2)
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

bool CCollisionMgr::CheckCollision2D_Rect_Circle(CCollider2D* _pColRect, CCollider2D* _pColCircle)
{
	return false;
}

bool CCollisionMgr::CheckCollision2D_Rect_OBB(CCollider2D* _pColRect, CCollider2D* _pColOBB)
{
	return false;
}

bool CCollisionMgr::CheckCollision2D_Rect_Point(CCollider2D* _pColRect, CCollider2D* _pColPoint)
{
	
	const Vec4& LBRT = _pColRect->GetSimpleCollider();
	const Vec2& Point = _pColPoint->GetCenterPos();

	enum LBRT{ L, B, R, T };
	if (Point.x < LBRT[L])
		return false;
	else if (Point.x > LBRT[R])
		return false;
	else if (Point.y < LBRT[B])
		return false;
	else if (Point.y > LBRT[T])
		return false;

	return true;
}

bool CCollisionMgr::CheckCollision2D_Circle_Circle(CCollider2D* _pColCircle_1, CCollider2D* _pColCircle_2)
{
	return false;
}

bool CCollisionMgr::CheckCollision2D_Circle_OBB(CCollider2D* _pColCircle, CCollider2D* _pColOBB)
{
	return false;
}

bool CCollisionMgr::CheckCollision2D_Circle_Point(CCollider2D* _pColCircle, CCollider2D* _pColPoint)
{
	return false;
}

bool CCollisionMgr::CheckCollision2D_OBB_OBB(CCollider2D* _pColOBB2D_1, CCollider2D* _pColOBB2D_2)
{
	const tOBB2D& OBB_1 = static_cast<CCollider2D_OBB*>(_pColOBB2D_1)->GetColliderInfo();
	const tOBB2D& OBB_2 = static_cast<CCollider2D_OBB*>(_pColOBB2D_2)->GetColliderInfo();

	//Vec2의 사이즈 * 2를 계산한다. 최적화를 위해 처음 계산한 값을 계속 사용.
	static const size_t vec2_2size = sizeof(Vec2) * 2;

	//각 축의 정보를 순회하기 편하도록 가져온다.
	//OBB 구조체의 가장 첫 부분에는 X축, Y축에 대한 정보가 들어 있음. memcpy로 가져와주면 됨.
	Vec2 arrVec[4] = {};
	memcpy_s(&arrVec[0], vec2_2size, &OBB_1, vec2_2size);
	memcpy_s(&arrVec[2], vec2_2size, &OBB_2, vec2_2size);


	Vec2 VecMiddle = OBB_1.m_vCenterPos - OBB_2.m_vCenterPos;
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

bool CCollisionMgr::CheckCollision2D_OBB_Point(CCollider2D* _pColOBB2D, CCollider2D* _pColPoint)
{
	const tOBB2D& OBBInfo = static_cast<CCollider2D_OBB*>(_pColOBB2D)->GetColliderInfo();
	Vec2 vPointToOBBCenter = _pColPoint->GetCenterPos();

	//점의 위치로부터 OBB의 중심 위치까지 弧娩?
	vPointToOBBCenter -= OBBInfo.m_vCenterPos;

	for (int i = 0; i < (int)eAXIS2D::END; ++i)
	{
		//사영의 대상이 될 축을 단위벡터로 전환
		Vec2 Norm = OBBInfo.m_vAxis[i];
		Norm.Normalize();

		//각 축을 사영 대상 축에 사영하고, 0.5를 곱해서 반 길이로 줄인다.(중심부터의 거리이므로)
		float NormLen = 0.f;
		NormLen += fabs(Norm.Dot(OBBInfo.m_vAxis[(int)eAXIS2D::X]));
		NormLen += fabs(Norm.Dot(OBBInfo.m_vAxis[(int)eAXIS2D::Y]));
		NormLen *= 0.5f;

		//마찬가지로 점과 OBB의 중심점도 같은 축에 사영한다.
		float PointToOBBCenterProj = fabs(Norm.Dot(vPointToOBBCenter));
		
		//만약 XY축을 사영한 길이가 중점 사이의 거리보다 길 경우 충돌이 아니다.
		if (PointToOBBCenterProj > NormLen)
			return false;
	}

	////X축 사영
	//Vec2 NormX = OBBInfo.m_vAxis[(int)eAXIS2D::X];
	//NormX.Normalize();
	//float NormXLen = 0.f;
	//NormXLen += fabs(NormX.Dot(OBBInfo.m_vAxis[(int)eAXIS2D::X]));
	//NormXLen += fabs(NormX.Dot(OBBInfo.m_vAxis[(int)eAXIS2D::Y]));
	//NormXLen *= 0.5f;

	//float PointToOBBCenterProjX = fabs(NormX.Dot(vPointToOBBCenter));

	//if (PointToOBBCenterProjX > NormXLen)
	//	return false;


	//Vec2 NormY = OBBInfo.m_vAxis[(int)eAXIS2D::Y];
	//NormY.Normalize();
	//float NormYLen = 0.f;
	//NormYLen += fabs(NormY.Dot(OBBInfo.m_vAxis[(int)eAXIS2D::X]));
	//NormYLen += fabs(NormY.Dot(OBBInfo.m_vAxis[(int)eAXIS2D::Y]));
	//NormYLen *= 0.5f;

	//float PointToOBBCenterProjY = fabs(NormY.Dot(vPointToOBBCenter));

	//if (PointToOBBCenterProjY > NormYLen)
	//	return false;


	return true;
}


bool CCollisionMgr::CheckCollision2D_Point_Point(CCollider2D* _pColPoint_1, CCollider2D* _pColPoint_2)
{
	return false;
}

void CCollisionMgr::RegisterCollisionFunc()
{
	//인덱스와 함수가 순서가 불일치할 경우 std::placeholders를 역순으로 배치할것

	//RECTANGLE
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::RECT][(int)eCOLLIDER_TYPE_2D::RECT]
		= std::bind(&CCollisionMgr::CheckCollision2D_Rect_Rect, this, std::placeholders::_1, std::placeholders::_2);
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::RECT][(int)eCOLLIDER_TYPE_2D::CIRCLE]
		= std::bind(&CCollisionMgr::CheckCollision2D_Rect_Circle, this, std::placeholders::_1, std::placeholders::_2);
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::RECT][(int)eCOLLIDER_TYPE_2D::OBB]
		= std::bind(&CCollisionMgr::CheckCollision2D_Rect_OBB, this, std::placeholders::_1, std::placeholders::_2);
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::RECT][(int)eCOLLIDER_TYPE_2D::POINT]
		= std::bind(&CCollisionMgr::CheckCollision2D_Rect_Point, this, std::placeholders::_1, std::placeholders::_2);

	//CIRCLE
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::CIRCLE][(int)eCOLLIDER_TYPE_2D::RECT]
		= std::bind(&CCollisionMgr::CheckCollision2D_Rect_Circle, this, std::placeholders::_2, std::placeholders::_1);
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::CIRCLE][(int)eCOLLIDER_TYPE_2D::CIRCLE]
		= std::bind(&CCollisionMgr::CheckCollision2D_Circle_Circle, this, std::placeholders::_1, std::placeholders::_2);
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::CIRCLE][(int)eCOLLIDER_TYPE_2D::OBB]
		= std::bind(&CCollisionMgr::CheckCollision2D_Circle_OBB, this, std::placeholders::_1, std::placeholders::_2);
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::CIRCLE][(int)eCOLLIDER_TYPE_2D::POINT]
		= std::bind(&CCollisionMgr::CheckCollision2D_Circle_Point, this, std::placeholders::_1, std::placeholders::_2);

	//OBB
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::OBB][(int)eCOLLIDER_TYPE_2D::RECT]
		= std::bind(&CCollisionMgr::CheckCollision2D_Rect_OBB, this, std::placeholders::_2, std::placeholders::_1);
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::OBB][(int)eCOLLIDER_TYPE_2D::CIRCLE]
		= std::bind(&CCollisionMgr::CheckCollision2D_Circle_OBB, this, std::placeholders::_2, std::placeholders::_1);
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::OBB][(int)eCOLLIDER_TYPE_2D::OBB]
		= std::bind(&CCollisionMgr::CheckCollision2D_OBB_OBB, this, std::placeholders::_1, std::placeholders::_2);
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::OBB][(int)eCOLLIDER_TYPE_2D::POINT]
		= std::bind(&CCollisionMgr::CheckCollision2D_OBB_Point, this, std::placeholders::_1, std::placeholders::_2);


	//POINT
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::POINT][(int)eCOLLIDER_TYPE_2D::RECT]
		= std::bind(&CCollisionMgr::CheckCollision2D_Rect_Point, this, std::placeholders::_2, std::placeholders::_1);
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::POINT][(int)eCOLLIDER_TYPE_2D::CIRCLE]
		= std::bind(&CCollisionMgr::CheckCollision2D_Circle_Point, this, std::placeholders::_2, std::placeholders::_1);
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::POINT][(int)eCOLLIDER_TYPE_2D::OBB]
		= std::bind(&CCollisionMgr::CheckCollision2D_OBB_Point, this, std::placeholders::_2, std::placeholders::_1);
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::POINT][(int)eCOLLIDER_TYPE_2D::POINT]
		= std::bind(&CCollisionMgr::CheckCollision2D_Point_Point, this, std::placeholders::_1, std::placeholders::_2);
}
