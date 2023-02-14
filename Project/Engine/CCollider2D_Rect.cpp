#include "pch.h"
#include "CCollider2D_Rect.h"

#include "CTransform.h"

CCollider2D_Rect::CCollider2D_Rect()
	: CCollider2D(eCOLLIDER_TYPE::_2D_RECT)
	, m_tRectInfo{}
{
}

CCollider2D_Rect::CCollider2D_Rect(eCOLLIDER_TYPE _Type)
	: CCollider2D(_Type)
	, m_tRectInfo{}
{
}

CCollider2D_Rect::CCollider2D_Rect(const CCollider2D_Rect& _other)
	: CCollider2D(_other)
	, m_tRectInfo(_other.m_tRectInfo)
{
}

CCollider2D_Rect::~CCollider2D_Rect()
{
}

void CCollider2D_Rect::UpdateCollider()
{
	CTransform* pTransform = GetOwner()->Transform();
	assert(nullptr != pTransform);

	//간이 충돌체 정보도 업데이트 한다.
	float SideLenHalf = pTransform->GetAABBSideLen();

	Vec2 vCenter = GetCenterPos();

	m_tRectInfo.fLeft = vCenter.x - SideLenHalf;
	m_tRectInfo.fBottom = vCenter.y - SideLenHalf;

	float SideLen = SideLenHalf * 2.f;
	m_tRectInfo.fRight = m_tRectInfo.fLeft + SideLen;
	m_tRectInfo.fTop = m_tRectInfo.fBottom + SideLen;
}

void CCollider2D_Rect::UpdateAABBinfo()
{

}

void CCollider2D_Rect::UpdateSpatialPartitionInfo(vector<Vec2>& _vecSpatialPartitonVtx)
{
	_vecSpatialPartitonVtx.push_back(Vec2(m_tRectInfo.fLeft, m_tRectInfo.fTop));
	_vecSpatialPartitonVtx.push_back(Vec2(m_tRectInfo.fRight, m_tRectInfo.fTop));
	_vecSpatialPartitonVtx.push_back(Vec2(m_tRectInfo.fRight, m_tRectInfo.fBottom));
	_vecSpatialPartitonVtx.push_back(Vec2(m_tRectInfo.fLeft, m_tRectInfo.fBottom));
}

void CCollider2D_Rect::DebugRender()
{
}
