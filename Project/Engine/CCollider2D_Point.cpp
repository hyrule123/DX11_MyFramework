#include "pch.h"
#include "CCollider2D_Point.h"

CCollider2D_Point::CCollider2D_Point()
	: CCollider2D(eCOLLIDER_TYPE::_2D_POINT)
{
}

CCollider2D_Point::~CCollider2D_Point()
{
}


void CCollider2D_Point::UpdateCollider()
{
}

void CCollider2D_Point::UpdateSpatialPartitionInfo(vector<Vec2>& _vecSpatialPartitonVtx)
{
}

void CCollider2D_Point::DebugRender()
{
}

