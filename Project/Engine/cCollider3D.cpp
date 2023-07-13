#include "pch.h"
#include "cCollider3D.h"

#include "cCollisionMgr.h"

#include "cCollider2D_Rect.h"
#include "cCollider2D_Circle.h"
#include "cCollider2D_OBB.h"


cCollider3D::cCollider3D(eCOLLIDER_TYPE_3D _eColType)
	: cCollider(eCOMPONENT_TYPE::COLLIDER3D, eDIMENSION_TYPE::_3D)
	, m_eColType(_eColType)
{
}


cCollider3D::~cCollider3D()
{
}

void cCollider3D::finaltick()
{
	//아래의 두 메소드는 transform에서 하는 걸로 변경
	//UpdatecColliderInfo();
	//UpdateSpatialPartitionInfo();
	DebugRender();

	//cCollisionMgr::GetInst()->AddcCollider2D(this, GetSpatialPartitionInfo());
}
