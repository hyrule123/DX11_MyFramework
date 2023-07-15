#include "pch.h"
#include "cCom_Coll3D_Ray.h"

cCom_Coll3D_Ray::cCom_Coll3D_Ray()
	: ICollider3D(eCOLLIDER_TYPE_3D::LINE)
	, m_fLength()
{
}

cCom_Coll3D_Ray::~cCom_Coll3D_Ray()
{
}


void cCom_Coll3D_Ray::UpdatecColliderInfo()
{
}

void cCom_Coll3D_Ray::DebugRender()
{
}
