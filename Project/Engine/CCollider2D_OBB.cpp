#include "pch.h"
#include "CCollider2D_OBB.h"

#include "CTransform.h"

//디버그 출력용
#include "CRenderMgr.h"

CCollider2D_OBB::CCollider2D_OBB()
	: CCollider2D_Rect(eCOLLIDER_TYPE::_2D_OBB)
	, m_tOBBInfo{}
{
}

CCollider2D_OBB::CCollider2D_OBB(const CCollider2D_OBB& _other)
	: CCollider2D_Rect(_other)
{
}

CCollider2D_OBB::~CCollider2D_OBB()
{
}


//이 함수는 CTransform에서 값이 변했을 경우에만 호출된다.
void CCollider2D_OBB::UpdateCollider()
{
	//자신의 부모 충돌체를 간이 충돌체로 사용한다.
	CCollider2D_Rect::UpdateCollider();

	//충돌체 주소를 가져온다.
	CTransform* pTransform = GetOwner()->Transform();
	assert(nullptr != pTransform);

	//자신의 OBB 정보를 계산한다.
	const Matrix& WorldMat = pTransform->GetWorldMat();

	const Vec3& Size = pTransform->GetSize();
	
	for (int i = 0; i < (UINT)eAXIS2D::END; ++i)
	{
		m_tOBBInfo.m_vAxis[i] = Vec3::TransformNormal(Vec3::Unit[i], WorldMat) * Size[i];
	}
	
	//중심점 = 월드 위치 + 오프셋 위치
	m_tOBBInfo.m_vMiddle = WorldMat.Translation() + GetOffsetPos();
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
	Info.eShapeType = (int)eSHAPE_TYPE::RECT;
	Info.matWorld = matScale * matOffset * matSize * matWorld;

	//충돌 중인 물체가 있을 경우 빨강, 아닐 경우 초록
	Info.vColor = 0 == GetCollisionCount() ? Vec4(0.f, 1.f, 0.f, 1.f) : Vec4(1.f, 0.f, 0.f, 1.f);
	CRenderMgr::GetInst()->AddDebugShapeRender(Info);
}
