#include "pch.h"
#include "Collider3D.h"

#include "CollisionMgr.h"

#include "Collider2D_Rect.h"
#include "Collider2D_Circle.h"
#include "Collider2D_OBB.h"


Collider3D::Collider3D(eCOLLIDER_TYPE_3D _eColType)
	: Collider(eCOMPONENT_TYPE::COLLIDER3D, eDIMENSION_TYPE::_3D)
	, m_eColType(_eColType)
{
}


Collider3D::~Collider3D()
{
}

void Collider3D::finaltick()
{
	//아래의 두 메소드는 transform에서 하는 걸로 변경
	//UpdateColliderInfo();
	//UpdateSpatialPartitionInfo();
	DebugRender();

	//CollisionMgr::GetInst()->AddCollider2D(this, GetSpatialPartitionInfo());
}
