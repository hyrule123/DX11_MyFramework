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
	//트랜스폼의 월드행렬이 변경되어 자신의 충돌체 업데이트도 필요한 경우
	if (true == GetNeedCollUpdate())
	{
		//자신의 중심 위치를 구한다.
		const Vec3& WorldPos = Transform()->GetWorldPos();
		const Vec3& OffsetPos = GetOffsetPos();
		m_vCenterPos.x = WorldPos.x + OffsetPos.x;
		m_vCenterPos.y = WorldPos.y + OffsetPos.y;

		//위치도 이동했을 수 있으므로 꼭짓점 정보도 갱신한다.
		m_vecSpatialPartitionVtx.clear();
		UpdateSpatialPartitionInfo(m_vecSpatialPartitionVtx);
		CCollisionMgr::GetInst()->AddCollider2D(this, m_vecSpatialPartitionVtx);

		//충돌체 업데이트 함수를 호출한다.
		UpdateCollider();
	}

	//간이 충돌체 업데이트가 필요할 경우
	if (true == GetNeedAABBUpdate())
	{
		UpdateAABBInfo();

		SetNeedAABBUpdate(false);
	}
	else
	{
		CCollisionMgr::GetInst()->AddCollider2D(this, m_vecGridIdxInfo);
	}




	//아래의 두 메소드는 transform에서 하는 걸로 변경
	//UpdateColliderInfo();
	//UpdateSpatialPartitionInfo();

	//에디터 카메라 모드일떄만 디버그
	if(true == CRenderMgr::GetInst()->IsEditorCamMode())
		DebugRender();
}



