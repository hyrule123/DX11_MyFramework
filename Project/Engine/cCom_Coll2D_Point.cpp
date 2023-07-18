#include "pch.h"
#include "cCom_Coll2D_Point.h"

#include "cGameObject.h"
#include "cTransform.h"
#include "cRenderMgr.h"

#include "S_H_Debug.hlsli"

cCom_Coll2D_Point::cCom_Coll2D_Point()
	: ICollider2D(eCOLLIDER_TYPE_2D::POINT)
{
}

cCom_Coll2D_Point::~cCom_Coll2D_Point()
{
}



void cCom_Coll2D_Point::UpdateSimplecCollider(Vec4& _vSimpleCollLBRTPos)
{
	const Vec3& vCenterPos = GetCenterPos();

	_vSimpleCollLBRTPos = Vec4(vCenterPos.x, vCenterPos.y, vCenterPos.x, vCenterPos.y);
}



void cCom_Coll2D_Point::DebugRender()
{
	const cTransform& pTransform = GetOwner()->Transform();
		
	const Matrix& matOffset = Matrix::CreateTranslation(GetCenterPos());

	//20 사이즈의 작은 원을 생성
	const static Matrix matSize = Matrix::CreateScale(20.f);

	tDebugShapeInfo Info = {};
	Info.eShapeType = (int)eDEBUGSHAPE_TYPE::CIRCLE;
	
	Info.matWorld = matSize * matOffset;

	//충돌 중인 물체가 있을 경우 빨강, 아닐 경우 초록
	Info.vColor = 0 == GetCollisionCount() ? Vec4(0.f, 1.f, 0.f, 1.f) : Vec4(1.f, 0.f, 0.f, 1.f);
	cRenderMgr::GetInst()->AddDebugShapeRender(Info);
}

