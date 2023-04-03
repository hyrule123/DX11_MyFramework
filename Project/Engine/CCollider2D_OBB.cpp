#include "pch.h"
#include "CCollider2D_OBB.h"

#include "CTransform.h"

//디버그 출력용
#include "CRenderMgr.h"

#include "S_0_H_Debug.hlsli"

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


//이 함수는 CTransform에서 값이 변했을 경우에만 호출된다.
void CCollider2D_OBB::UpdateCollider()
{
	//충돌체 주소를 가져온다.
	CTransform* pTransform = GetOwner()->Transform();
	assert(nullptr != pTransform);
	//자신의 OBB 정보를 계산한다.

	const Matrix& WorldMat = pTransform->GetWorldMatWithoutSize();
	const Vec3& Size = GetCollSize();

	for (int i = 0; i < (int)eAXIS2D::END; ++i)
	{
		//단위벡터로 만든 뒤 size를 곱해준다.
		m_tOBBInfo.m_v2Axis[i] = Vec3::TransformNormal(Vec3::Unit[i], WorldMat) * Size[i];
	}

	//부모의 동일함수를 호출(Simple Collider 계산)
	CCollider2D::UpdateCollider();
}

void CCollider2D_OBB::UpdateSimpleCollider(Vec4& _vSimpleCollLBRTPos)
{
	const Vec2& CenterPos = GetCenterPos().XY();
	_vSimpleCollLBRTPos = Vec4(CenterPos, CenterPos);

	Vec2 size = GetCollSize().XY();
	//제곱
	size *= size;
	//대각선 길이를 구한다.
	float diagLen = sqrtf(size.x + size.y) * 0.5f;

	_vSimpleCollLBRTPos += Vec4(-diagLen, -diagLen, diagLen, diagLen);
}


void CCollider2D_OBB::DebugRender()
{
	//충돌체(자신)의 정보)
	const Matrix& matSize = Matrix::CreateScale(GetCollSize());

	float angle = atan2f(m_tOBBInfo.m_v2Axis[0].y, m_tOBBInfo.m_v2Axis[0].x);

	const Matrix& matRot = Matrix::CreateFromQuaternion(Quaternion::CreateFromYawPitchRoll(0.f, 0.f, angle));

	const Vec3& offsetPos = GetCenterPos();

	tDebugShapeInfo Info = {};
	Info.eShapeType = (int)eDEBUGSHAPE_TYPE::RECT;
	Info.matWorld = matSize * matRot;

	//offsetPos만큼 이동
	Info.matWorld._41 += offsetPos.x;
	Info.matWorld._42 += offsetPos.y;
	Info.matWorld._43 += offsetPos.z;

	//충돌 중인 물체가 있을 경우 빨강, 아닐 경우 초록
	Info.vColor = 0 == GetCollisionCount() ? Vec4(0.f, 1.f, 0.f, 1.f) : Vec4(1.f, 0.f, 0.f, 1.f);
	CRenderMgr::GetInst()->AddDebugShapeRender(Info);
}
