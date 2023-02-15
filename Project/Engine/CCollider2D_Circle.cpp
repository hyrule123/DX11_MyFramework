#include "pch.h"
#include "CCollider2D_Circle.h"

CCollider2D_Circle::CCollider2D_Circle()
	: CCollider2D(eCOLLIDER_TYPE_2D::CIRCLE)
{
}

CCollider2D_Circle::CCollider2D_Circle(const CCollider2D_Circle& _other)
	: CCollider2D(_other)
{
}

CCollider2D_Circle::~CCollider2D_Circle()
{
}

void CCollider2D_Circle::UpdateCollider()
{
}

void CCollider2D_Circle::DebugRender()
{
}
