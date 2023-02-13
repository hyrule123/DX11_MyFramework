#include "pch.h"
#include "CCollider2D_Point.h"

CCollider2D_Point::CCollider2D_Point()
{
}

CCollider2D_Point::~CCollider2D_Point()
{
}

bool CCollider2D_Point::CheckCollisionRect(CCollider2D_Rect* _other)
{
	return false;
}

bool CCollider2D_Point::CheckCollisionCircle(CCollider2D_Circle* _other)
{
	return false;
}

bool CCollider2D_Point::CheckCollisionOBB2D(CCollider2D_OBB* _other)
{
	return false;
}

void CCollider2D_Point::UpdateColliderInfo()
{
}

void CCollider2D_Point::DebugRender()
{
}

