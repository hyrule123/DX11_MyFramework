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




	//아래의 두 메소드는 transform에서 하는 걸로 변경
	//UpdateColliderInfo();
	//UpdateSpatialPartitionInfo();

	//에디터 카메라 모드일떄만 디버그
	if(true == CRenderMgr::GetInst()->IsEditorCamMode())
		DebugRender();

	if()
		CCollisionMgr::GetInst()->AddCollider2D(this, GetSpatialPartitionInfo());
}

void CCollider2D::UpdateCollider()
{


	//간이 충돌체 정보도 업데이트 한다.
	float SideLenHalf = pTransform->GetAABBSideLen();

	tRectInfo Info = {};
	Info.LB.x = m_tOBBInfo.m_vMiddle.x - SideLenHalf;
	Info.LB.y = m_tOBBInfo.m_vMiddle.y - SideLenHalf;

	float SideLen = SideLenHalf * 2.f;
	Info.RT = Info.LB;
	Info.RT += SideLen;

	SetSpatialPartitionInfo(Info);
}



