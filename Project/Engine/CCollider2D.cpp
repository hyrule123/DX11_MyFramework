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

	//Ʈ�������� ��������� ����Ǿ� �ڽ��� �浹ü ������Ʈ�� �ʿ��� ���
	if (true == pTransform->IsUpdated())
	{
		//�ڽ��� �߽� ��ġ�� ���Ѵ�.
		const Vec3& WorldPos = Transform()->GetWorldPos();
		const Vec3& OffsetPos = GetOffsetPos();
		m_vCenterPos.x = WorldPos.x + OffsetPos.x;
		m_vCenterPos.y = WorldPos.y + OffsetPos.y;

		bNeedAABBUpdate = true;

		//�ڽ��� ������ ������Ʈ �ϰ� ���� �浹ü ������Ʈ �Լ��� ȣ���Ѵ�.
		UpdateCollider();
	}

	//�Ǵ� Ʈ�������� ���������� ������Ʈ ���� �ʾ����� Ʈ�������� ����� ������Ʈ�Ǿ��� ��� 
	bNeedAABBUpdate |= pTransform->GetSizeUpdated();
	//���� �浹ü ������Ʈ�� �ʿ��� ���
	if (true == bNeedAABBUpdate)
	{
		//���� �浹ü ������ �����Ѵ�. �⺻ ������ Transform���� ���� ���̸� �޾ƿͼ� ���簢�� ���·� ������.
		UpdateSimpleCollider(m_vSimpleCollLBRTPos);

		//��ġ LBRT ������ �����ؼ� �ε��� ������ �޾ƿ´�.
		CCollisionMgr::GetInst()->CalcAndAddCollider2D(this, m_vSimpleCollLBRTPos, m_vecGridIdxInfo);
	}

	//�� �ʿ� ���� ��� 
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

void CCollider2D::UpdateSimpleCollider(Vec4& _vSimpleCollLBRTPos)
{
	CTransform* pTransform = Transform();

	if (nullptr != pTransform)
	{
		//�� ���� Ʈ�������� ũ��� ����� ��� ����� ����.
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



