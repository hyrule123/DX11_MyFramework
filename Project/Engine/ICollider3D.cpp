#include "pch.h"
#include "ICollider3D.h"

#include "cCollisionMgr.h"

#include "cCom_Coll2D_Rect.h"
#include "cCom_Coll2D_Circle.h"
#include "cCom_Coll2D_OBB.h"


ICollider3D::ICollider3D(eCOLLIDER_TYPE_3D _eColType)
	: ICollider(eDIMENSION_TYPE::_3D)
	, m_eColType(_eColType)
{
}


ICollider3D::~ICollider3D()
{
}

void ICollider3D::FinalTick()
{
	//아래의 두 메소드는 transform에서 하는 걸로 변경
	//UpdatecColliderInfo();
	//UpdateSpatialPartitionInfo();
	DebugRender();

	//cCollisionMgr::GetInst()->AddcCollider2D(this, GetSpatialPartitionInfo());
}
