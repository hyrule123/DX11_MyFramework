#include "pch.h"
#include "cCollider3D_Ray.h"

cCollider3D_Ray::cCollider3D_Ray()
	: cCollider3D(eCOLLIDER_TYPE_3D::LINE)
	, m_fLength()
{
}

cCollider3D_Ray::~cCollider3D_Ray()
{
}


void cCollider3D_Ray::UpdatecColliderInfo()
{
}

void cCollider3D_Ray::DebugRender()
{
}
