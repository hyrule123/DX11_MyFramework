#include "pch.h"
#include "CCollider2D_OBB.h"

#include "CTransform.h"

//����� ��¿�
#include "CRenderMgr.h"

CCollider2D_OBB::CCollider2D_OBB()
	: CCollider2D(eCOLLIDER_TYPE_2D::OBB)
	, m_tOBBInfo{}
{
}

CCollider2D_OBB::CCollider2D_OBB(const CCollider2D_OBB& _other)
	: CCollider2D(_other)
{
}

CCollider2D_OBB::~CCollider2D_OBB()
{
}


//�� �Լ��� CTransform���� ���� ������ ��쿡�� ȣ��ȴ�.
void CCollider2D_OBB::UpdateCollider()
{
	//�浹ü �ּҸ� �����´�.
	CTransform* pTransform = GetOwner()->Transform();
	assert(nullptr != pTransform);


	//�ڽ��� OBB ������ ����Ѵ�.
	const Matrix& WorldMat = pTransform->GetWorldMat();
	const Vec3& Size = pTransform->GetSize();

	for (int i = 0; i < (int)eAXIS2D::END; ++i)
	{
		m_tOBBInfo.m_vAxis[i] = Vec3::TransformNormal(Vec3::Unit[i], WorldMat) * Size[i];
	}

	//�߽��� = ���� ��ġ + ������ ��ġ
	m_tOBBInfo.m_vCenterPos = GetCenterPos();

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
