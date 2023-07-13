#include "pch.h"
#include "Collider2D_Point.h"


#include "Transform.h"
#include "RenderMgr.h"

#include "S_H_Debug.hlsli"

Collider2D_Point::Collider2D_Point()
	: Collider2D(eCOLLIDER_TYPE_2D::POINT)
{
}

Collider2D_Point::~Collider2D_Point()
{
}



void Collider2D_Point::UpdateSimpleCollider(Vec4& _vSimpleCollLBRTPos)
{
	const Vec3& vCenterPos = GetCenterPos();

	_vSimpleCollLBRTPos = Vec4(vCenterPos.x, vCenterPos.y, vCenterPos.x, vCenterPos.y);
}



void Collider2D_Point::DebugRender()
{
	const Transform& pTransform = Transform();
		
	const Matrix& matOffset = Matrix::CreateTranslation(GetCenterPos());

	//20 사이즈의 작은 원을 생성
	const static Matrix matSize = Matrix::CreateScale(20.f);

	tDebugShapeInfo Info = {};
	Info.eShapeType = (int)eDEBUGSHAPE_TYPE::CIRCLE;
	
	Info.matWorld = matSize * matOffset;

	//충돌 중인 물체가 있을 경우 빨강, 아닐 경우 초록
	Info.vColor = 0 == GetCollisionCount() ? Vec4(0.f, 1.f, 0.f, 1.f) : Vec4(1.f, 0.f, 0.f, 1.f);
	RenderMgr::GetInst()->AddDebugShapeRender(Info);
}

