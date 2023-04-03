#include "pch.h"
#include "CCollider2D_Circle.h"

#include "CTransform.h"
#include "S_0_H_Debug.hlsli"
#include "CRenderMgr.h"

#include "strKeyDefault.h"

#include "jsoncpp.h"

CCollider2D_Circle::CCollider2D_Circle()
	: CCollider2D(eCOLLIDER_TYPE_2D::CIRCLE)
	//, m_fRadius(100.f)
{
}

CCollider2D_Circle::CCollider2D_Circle(const CCollider2D_Circle& _other)
	: CCollider2D(_other)
	//, m_fRadius(_other.m_fRadius)
{
}

CCollider2D_Circle::~CCollider2D_Circle()
{
}

bool CCollider2D_Circle::SaveJson(Json::Value* _pJVal)
{
	if (nullptr == _pJVal)
		return false;
	else if (false == CCollider2D::SaveJson(_pJVal))
		return false;

	Json::Value& jVal = *_pJVal;

	//jVal[string(RES_INFO::PREFAB::COMPONENT::COLLIDER2D::CIRCLE::JSON_KEY::m_fRadius)] = m_fRadius;

	return true;
}

bool CCollider2D_Circle::LoadJson(Json::Value* _pJVal)
{
	if (nullptr == _pJVal)
		return false;
	else if (false == CCollider2D::LoadJson(_pJVal))
		return false;

	Json::Value& jVal = *_pJVal;

	//string strKey = string(RES_INFO::PREFAB::COMPONENT::COLLIDER2D::CIRCLE::JSON_KEY::m_fRadius);
	//if (jVal.isMember(strKey))
	//	m_fRadius = jVal[strKey].asFloat();
	//else
	//	return false;

	return true;
}


void CCollider2D_Circle::UpdateSimpleCollider(Vec4& _vSimpleCollLBRTPos)
{
	const Vec2& v2Pos = GetCenterPos().XY();

	//반지름은 X값만을 사용.
	float fRadius = GetCollSize().x;

	//타원은 지원하지 않음
	Vec2 LBRT(fRadius, fRadius);

	_vSimpleCollLBRTPos = Vec4(v2Pos - LBRT, v2Pos + LBRT);
}

void CCollider2D_Circle::DebugRender()
{
	CTransform* pTransform = Transform();

	const Matrix& matOffset = Matrix::CreateTranslation(GetCenterPos());

	//원을 생성
	const Matrix& matSize = Matrix::CreateScale(GetRadius() * 2.f);

	tDebugShapeInfo Info = {};
	Info.eShapeType = (int)eDEBUGSHAPE_TYPE::CIRCLE;

	Info.matWorld = matSize * matOffset;

	//충돌 중인 물체가 있을 경우 빨강, 아닐 경우 초록
	Info.vColor = 0 == GetCollisionCount() ? Vec4(0.f, 1.f, 0.f, 1.f) : Vec4(1.f, 0.f, 0.f, 1.f);
	CRenderMgr::GetInst()->AddDebugShapeRender(Info);
}
