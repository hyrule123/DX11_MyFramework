#include "pch.h"
#include "CCollider2D_OBB.h"

#include "CTransform.h"

//디버그 출력용
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

	size_t size = sizeof(Vec2) * 2;

	//각 축의 정보를 순회하기 편하도록 가져온다.
	Vec2 arrVec[4] = {};
	memcpy_s(&arrVec[0], size, &m_tOBBInfo, size);
	memcpy_s(&arrVec[2], size, &otherInfo, size);

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
		
		//한 변의 길이(vSize)가 곱해졌으므로 0.5를 곱해줘야 실제 축의 길이가 됨
		fProjSum *= 0.5f;

		float fCenterProj = fabsf(norm.Dot(VecMiddle));

		if (fProjSum < fCenterProj)
			return false;
	}

	return true;
}

void CCollider2D_OBB::UpdateColliderInfo()
{
	//충돌체 정보를 업데이트 하고
	CTransform* pTransform = GetOwner()->Transform();
	assert(nullptr != pTransform);

	//자신의 OBB 정보를 계산한다.
	const Matrix& WorldMat = pTransform->GetWorldMat();

	const Vec3& Size = pTransform->GetSize();
	
	for (int i = 0; i < eAXIS2D_END; ++i)
	{
		m_tOBBInfo.m_vAxis[i] = Vec3::TransformNormal(Vec3::Unit[i], WorldMat) * Size[i];
	}
	
	//중심점 = 월드 위치 + 오프셋 위치
	m_tOBBInfo.m_vMiddle = WorldMat.Translation() + GetOffsetPos();


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

void CCollider2D_OBB::DebugRender()
{
	//나중에 디버그 정보가 필요할 경우에만 렌더링하도록 해줄것

	CTransform* pTransform = Transform();
	//트랜스폼의 정보
	const Matrix& matSize = Matrix::CreateScale(pTransform->GetSize());
	const Matrix& matWorld = pTransform->GetWorldMat();

	//충돌체(자신)의 정보)
	const Matrix& matScale = Matrix::CreateScale(GetOffsetScale());
	const Matrix& matOffset = Matrix::CreateTranslation(GetOffsetPos());

	tDebugShapeInfo Info = {};
	Info.eShape = eSHAPE_RECT;
	Info.matWorld = matScale * matOffset * matSize * matWorld;

	//충돌 중인 물체가 있을 경우 빨강, 아닐 경우 초록
	Info.vColor = 0 == GetCollisionCount() ? Vec4(0.f, 1.f, 0.f, 1.f) : Vec4(1.f, 0.f, 0.f, 1.f);
	CRenderMgr::GetInst()->AddDebugShapeRender(Info);
}
