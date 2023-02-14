#include "pch.h"

#include "CCollisionMgr.h"

#include "CCollider2D.h"

#include "CCollider2D_Rect.h"
#include "CCollider2D_Circle.h"
#include "CCollider2D_OBB.h"

#include "CRenderMgr.h"

#include "CTransform.h"

CCollider2D::CCollider2D(eCOLLIDER_TYPE _eColType)
	: CCollider(eCOMPONENT_TYPE::COLLIDER2D, _eColType)
	, m_vecGridIdxInfo{}
{
}

CCollider2D::CCollider2D(const CCollider2D& _other)
	:CCollider(_other)
	, m_vecGridIdxInfo(_other.m_vecGridIdxInfo)
{
}

CCollider2D::~CCollider2D()
{
}

void CCollider2D::finaltick()
{
	//Ʈ�������� ��������� ����Ǿ� �ڽ��� �浹ü ������Ʈ�� �ʿ��� ���
	if (true == GetNeedCollUpdate())
	{
		//�ڽ��� �߽� ��ġ�� ���Ѵ�.
		const Vec3& WorldPos = Transform()->GetWorldPos();
		const Vec3& OffsetPos = GetOffsetPos();
		m_vCenterPos.x = WorldPos.x + OffsetPos.x;
		m_vCenterPos.y = WorldPos.y + OffsetPos.y;

		//��ġ�� �̵����� �� �����Ƿ� ������ ������ �����Ѵ�.
		m_vecSpatialPartitionVtx.clear();
		UpdateSpatialPartitionInfo(m_vecSpatialPartitionVtx);
		CCollisionMgr::GetInst()->AddCollider2D(this, m_vecSpatialPartitionVtx);

		//�浹ü ������Ʈ �Լ��� ȣ���Ѵ�.
		UpdateCollider();
	}

	//���� �浹ü ������Ʈ�� �ʿ��� ���
	if (true == GetNeedAABBUpdate())
	{
		UpdateAABBInfo();

		SetNeedAABBUpdate(false);
	}
	else
	{
		CCollisionMgr::GetInst()->AddCollider2D(this, m_vecGridIdxInfo);
	}




	//�Ʒ��� �� �޼ҵ�� transform���� �ϴ� �ɷ� ����
	//UpdateColliderInfo();
	//UpdateSpatialPartitionInfo();

	//������ ī�޶� ����ϋ��� �����
	if(true == CRenderMgr::GetInst()->IsEditorCamMode())
		DebugRender();
}



