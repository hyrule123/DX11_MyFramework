#include "pch.h"

#include "CCollisionMgr.h"

#include "CCollider2D.h"

#include "CCollider2D_Rect.h"
#include "CCollider2D_Circle.h"
#include "CCollider2D_OBB.h"

#include "CRenderMgr.h"

CCollider2D::CCollider2D(eCOLLIDER_TYPE _eColType)
	: CCollider(eCOMPONENT_TYPE::COLLIDER2D, _eColType)
	, m_SpatialPartitionInfo{}
{
}

CCollider2D::CCollider2D(const CCollider2D& _other)
	:CCollider(_other)
	, m_SpatialPartitionInfo(_other.m_SpatialPartitionInfo)
{
}

CCollider2D::~CCollider2D()
{
}

void CCollider2D::finaltick()
{




	//�Ʒ��� �� �޼ҵ�� transform���� �ϴ� �ɷ� ����
	//UpdateColliderInfo();
	//UpdateSpatialPartitionInfo();

	//������ ī�޶� ����ϋ��� �����
	if(true == CRenderMgr::GetInst()->IsEditorCamMode())
		DebugRender();

	if()
		CCollisionMgr::GetInst()->AddCollider2D(this, GetSpatialPartitionInfo());
}

void CCollider2D::UpdateCollider()
{


	//���� �浹ü ������ ������Ʈ �Ѵ�.
	float SideLenHalf = pTransform->GetAABBSideLen();

	tRectInfo Info = {};
	Info.LB.x = m_tOBBInfo.m_vMiddle.x - SideLenHalf;
	Info.LB.y = m_tOBBInfo.m_vMiddle.y - SideLenHalf;

	float SideLen = SideLenHalf * 2.f;
	Info.RT = Info.LB;
	Info.RT += SideLen;

	SetSpatialPartitionInfo(Info);
}



