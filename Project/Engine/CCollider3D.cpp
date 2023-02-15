#include "pch.h"
#include "CCollider3D.h"

#include "CCollisionMgr.h"

#include "CCollider2D_Rect.h"
#include "CCollider2D_Circle.h"
#include "CCollider2D_OBB.h"


CCollider3D::CCollider3D(eCOLLIDER_TYPE_3D _eColType)
	: CCollider(eCOMPONENT_TYPE::COLLIDER3D)
	, m_eColType(_eColType)
{
}


CCollider3D::~CCollider3D()
{
}

void CCollider3D::finaltick()
{
	//아래의 두 메소드는 transform에서 하는 걸로 변경
	//UpdateColliderInfo();
	//UpdateSpatialPartitionInfo();
	DebugRender();

	//CCollisionMgr::GetInst()->AddCollider2D(this, GetSpatialPartitionInfo());
}