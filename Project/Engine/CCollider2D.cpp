#include "pch.h"

#include "CCollisionMgr.h"

#include "CCollider2D.h"

#include "CCollider2D_Rect.h"
#include "CCollider2D_Circle.h"
#include "CCollider2D_OBB.h"

#include "CRenderMgr.h"

#include "CTransform.h"

CCollider2D::CCollider2D(eCOLLIDER_TYPE_2D _eColType)
	: CCollider(eCOMPONENT_TYPE::COLLIDER2D)
	, m_eColType(_eColType)
	, m_vecGridIdxInfo{}
{
}


CCollider2D::~CCollider2D()
{
}

void CCollider2D::finaltick()
{
	CTransform* pTransform = Transform();

	if (nullptr == pTransform)
		return;

	bool bNeedAABBUpdate = false;

	//트랜스폼의 월드행렬이 변경되어 자신의 충돌체 업데이트가 필요한 경우
	if (true == pTransform->IsUpdated())
	{
		//자신의 중심 위치를 구한다.
		const Vec3& WorldPos = Transform()->GetWorldPos();
		const Vec3& OffsetPos = GetOffsetPos();
		m_vCenterPos.x = WorldPos.x + OffsetPos.x;
		m_vCenterPos.y = WorldPos.y + OffsetPos.y;

		bNeedAABBUpdate = true;

		//자신의 정보를 업데이트 하고 나면 충돌체 업데이트 함수를 호출한다.
		UpdateCollider();
	}

	//또는 트랜스폼의 월드정보는 업데이트 되지 않았지만 트랜스폼의 사이즈가 업데이트되었을 경우 
	bNeedAABBUpdate |= pTransform->GetSizeUpdated();
	//간이 충돌체 업데이트가 필요할 경우
	if (true == bNeedAABBUpdate)
	{
		//간이 충돌체 정보만 갱신한다. 기본 설정은 Transform에서 변의 길이를 받아와서 정사각형 형태로 생성함.
		UpdateSimpleCollider(m_vSimpleCollLBRTPos);

		//위치 LBRT 정보를 전달해서 인덱스 정보를 받아온다.
		CCollisionMgr::GetInst()->CalcAndAddCollider2D(this, m_vSimpleCollLBRTPos, m_vecGridIdxInfo);
	}

	//다 필요 없을 경우 
	else
	{
		CCollisionMgr::GetInst()->AddCollider2D(this, m_vecGridIdxInfo);
	}
	
	//아래의 두 메소드는 transform에서 하는 걸로 변경
	//UpdateColliderInfo();
	//UpdateSpatialPartitionInfo();

	//에디터 카메라 모드일만 디버그
	if(true == CRenderMgr::GetInst()->IsEditorCamMode())
		DebugRender();
}

void CCollider2D::UpdateSimpleCollider(Vec4& _vSimpleCollLBRTPos)
{
	CTransform* pTransform = Transform();

	if (nullptr != pTransform)
	{
		//이 값은 트랜스폼의 크기와 사이즈가 모두 적용된 값임.
		float Sidelen = pTransform->GetAABBSideLen();

		//LB
		_vSimpleCollLBRTPos.x = m_vCenterPos.x - Sidelen;
		_vSimpleCollLBRTPos.y = m_vCenterPos.y - Sidelen;

		//RT
		Sidelen *= 2.f;
		_vSimpleCollLBRTPos.z = _vSimpleCollLBRTPos.x + Sidelen;
		_vSimpleCollLBRTPos.w = _vSimpleCollLBRTPos.y + Sidelen;
	}
}



