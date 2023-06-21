#include "pch.h"
#include "CCollider3D_Ray.h"

CCollider3D_Ray::CCollider3D_Ray()
	: CCollider3D(eCOLLIDER_TYPE_3D::LINE)
	, m_fLength()
{
}

CCollider3D_Ray::~CCollider3D_Ray()
{
}


void CCollider3D_Ray::UpdateColliderInfo()
{
}

void CCollider3D_Ray::DebugRender()
{
}
