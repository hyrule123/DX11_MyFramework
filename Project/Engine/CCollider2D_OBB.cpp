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

bool CCollider2D_OBB::CheckCollisionRect(CCollider2D_Rect* _other)
{
	return false;
}

bool CCollider2D_OBB::CheckCollisionCircle(CCollider2D_Circle* _other)
{
	return false;
}

bool CCollider2D_OBB::CheckCollisionOBB2D(CCollider2D_OBB* _other)
{
	const tOBB2D& otherInfo = _other->GetOBBInfo();

	//Vec2�� ������ * 2�� �޾ƿ´�.
	static const size_t vec2_2size = sizeof(Vec2) * 2;

	//�� ���� ������ ��ȸ�ϱ� ���ϵ��� �����´�.
	Vec2 arrVec[4] = {};
	memcpy_s(&arrVec[0], vec2_2size, &m_tOBBInfo, vec2_2size);
	memcpy_s(&arrVec[2], vec2_2size, &otherInfo, vec2_2size);

	Vec2 VecMiddle = m_tOBBInfo.m_vMiddle - otherInfo.m_vMiddle;
	

	for (int i = 0; i < 4; ++i)
	{
		Vec2 norm = arrVec[i];
		norm.Normalize();

		float fProjSum = 0.f;
		for (int j = 0; j < 4; ++j)
		{
			fProjSum += fabsf(norm.Dot(arrVec[j]));
		}
		
		//�� ���� ����(vSize)�� ���������Ƿ� 0.5�� ������� ���� ���� ���̰� ��
		fProjSum *= 0.5f;

		float fCenterProj = fabsf(norm.Dot(VecMiddle));

		if (fProjSum < fCenterProj)
			return false;
	}

	return true;
}

bool CCollider2D_OBB::CheckCollisionPoint(CCollider2D_Point* _other)
{



	return false;
}

void CCollider2D_OBB::UpdateColliderInfo()
{
	//�浹ü ������ ������Ʈ �ϰ�
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
