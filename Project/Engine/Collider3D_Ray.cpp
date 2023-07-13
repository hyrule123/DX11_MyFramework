#include "pch.h"
#include "Collider3D_Ray.h"

Collider3D_Ray::Collider3D_Ray()
	: Collider3D(eCOLLIDER_TYPE_3D::LINE)
	, m_fLength()
{
}

Collider3D_Ray::~Collider3D_Ray()
{
}


void Collider3D_Ray::UpdateColliderInfo()
{
}

void Collider3D_Ray::DebugRender()
{
}
