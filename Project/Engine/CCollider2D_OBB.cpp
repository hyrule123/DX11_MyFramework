#include "pch.h"
#include "CCollider2D_OBB.h"

#include "CTransform.h"

//����� ��¿�
#include "CRenderMgr.h"

CCollider2D_OBB::CCollider2D_OBB()
	: CCollider2D(eCOLLIDER_TYPE::_2D_OBB)
	, m_tOBBInfo{}
{
}

CCollider2D_OBB::CCollider2D_OBB(const CCollider2D& _other)
	: CCollider2D(_other)
{
}

CCollider2D_OBB::~CCollider2D_OBB()
{
}


//�� �Լ��� CTransform���� ���� ������ ��쿡�� ȣ��ȴ�.
void CCollider2D_OBB::UpdateColliderInfo()
{
	//�浹ü �ּҸ� �����´�.
	CTransform* pTransform = GetOwner()->Transform();
	assert(nullptr != pTransform);

	//�ڽ��� OBB ������ ����Ѵ�.
	const Matrix& WorldMat = pTransform->GetWorldMat();

	const Vec3& Size = pTransform->GetSize();
	
	for (int i = 0; i < (UINT)eAXIS2D::END; ++i)
	{
		m_tOBBInfo.m_vAxis[i] = Vec3::TransformNormal(Vec3::Unit[i], WorldMat) * Size[i];
	}
	
	//�߽��� = ���� ��ġ + ������ ��ġ
	m_tOBBInfo.m_vMiddle = WorldMat.Translation() + GetOffsetPos();


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

void CCollider2D_OBB::DebugRender()
{
	//���߿� ����� ������ �ʿ��� ��쿡�� �������ϵ��� ���ٰ�

	CTransform* pTransform = Transform();
	//Ʈ�������� ����
	const Matrix& matSize = Matrix::CreateScale(pTransform->GetSize());
	const Matrix& matWorld = pTransform->GetWorldMat();

	//�浹ü(�ڽ�)�� ����)
	const Matrix& matScale = Matrix::CreateScale(GetOffsetScale());
	const Matrix& matOffset = Matrix::CreateTranslation(GetOffsetPos());

	tDebugShapeInfo Info = {};
	Info.eShapeType = (int)eSHAPE_TYPE::RECT;
	Info.matWorld = matScale * matOffset * matSize * matWorld;

	//�浹 ���� ��ü�� ���� ��� ����, �ƴ� ��� �ʷ�
	Info.vColor = 0 == GetCollisionCount() ? Vec4(0.f, 1.f, 0.f, 1.f) : Vec4(1.f, 0.f, 0.f, 1.f);
	CRenderMgr::GetInst()->AddDebugShapeRender(Info);
}
