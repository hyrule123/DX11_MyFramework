#include "pch.h"
#include "CCollider3D.h"

#include "CCollisionMgr.h"

#include "CCollider2D_Rect.h"
#include "CCollider2D_Circle.h"
#include "CCollider2D_OBB.h"

CCollider3D::CCollider3D(eCOLLIDER_TYPE _eColType)
	: CCollider(eCOMPONENT_TYPE::COLLIDER3D, _eColType)
{
}

CCollider3D::CCollider3D(const CCollider3D& _other)
	:CCollider(_other)
{
}

CCollider3D::~CCollider3D()
{
}

bool CCollider3D::CheckCollision(CCollider* _other)
{
	switch (GetColliderType())
	{

	}


	return false;
}

void CCollider3D::finaltick()
{
	//�Ʒ��� �� �޼ҵ�� transform���� �ϴ� �ɷ� ����
	//UpdateColliderInfo();
	//UpdateSpatialPartitionInfo();
	DebugRender();

	//CCollisionMgr::GetInst()->AddCollider2D(this, GetSpatialPartitionInfo());
}