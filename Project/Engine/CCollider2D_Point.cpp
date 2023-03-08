#include "pch.h"
#include "CCollider2D_Point.h"


#include "CTransform.h"
#include "CRenderMgr.h"

#include "S_0_H_Debug.hlsli"

CCollider2D_Point::CCollider2D_Point()
	: CCollider2D(eCOLLIDER_TYPE_2D::POINT)
{
}

CCollider2D_Point::~CCollider2D_Point()
{
}



void CCollider2D_Point::UpdateSimpleCollider(Vec4& _vSimpleCollLBRTPos)
{
	const Vec2& vCenterPos = GetCenterPos();

	_vSimpleCollLBRTPos = Vec4(vCenterPos.x, vCenterPos.y, vCenterPos.x, vCenterPos.y);
}



void CCollider2D_Point::DebugRender()
{
	CTransform* pTransform = Transform();

	const Matrix& matWorld = pTransform->GetWorldMatWithoutSize();
		
	const Matrix& matOffset = Matrix::CreateTranslation(GetOffsetPos());

	//20 사이즈의 작은 원을 생성
	const static Matrix matSize = Matrix::CreateScale(20.f);

	tDebugShapeInfo Info = {};
	Info.eShapeType = (int)eDEBUGSHAPE_TYPE::CIRCLE;
	
	Info.matWorld = matSize * matOffset * matWorld;

	//충돌 중인 물체가 있을 경우 빨강, 아닐 경우 초록
	Info.vColor = 0 == GetCollisionCount() ? Vec4(0.f, 1.f, 0.f, 1.f) : Vec4(1.f, 0.f, 0.f, 1.f);
	CRenderMgr::GetInst()->AddDebugShapeRender(Info);
}

