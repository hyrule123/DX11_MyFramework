#include "pch.h"
#include "Collider2D_Circle.h"

#include "Transform.h"
#include "S_H_Debug.hlsli"
#include "RenderMgr.h"

#include "strKey_Default.h"

#include "jsoncpp.h"

Collider2D_Circle::Collider2D_Circle()
	: Collider2D(eCOLLIDER_TYPE_2D::CIRCLE)
	//, m_fRadius(100.f)
{
}

Collider2D_Circle::Collider2D_Circle(const Collider2D_Circle& _other)
	: Collider2D(_other)
	//, m_fRadius(_other.m_fRadius)
{
}

Collider2D_Circle::~Collider2D_Circle()
{
}

bool Collider2D_Circle::SaveJson(Json::Value* _pJVal)
{
	if (nullptr == _pJVal)
		return false;
	else if (false == Collider2D::SaveJson(_pJVal))
		return false;

	Json::Value& jVal = *_pJVal;

	//jVal[string(RES_INFO::PREFAB::COMPONENT::COLLIDER2D::CIRCLE::JSON_KEY::m_fRadius)] = m_fRadius;

	return true;
}

bool Collider2D_Circle::LoadJson(Json::Value* _pJVal)
{
	if (nullptr == _pJVal)
		return false;
	else if (false == Collider2D::LoadJson(_pJVal))
		return false;

	Json::Value& jVal = *_pJVal;

	//string strKey = string(RES_INFO::PREFAB::COMPONENT::COLLIDER2D::CIRCLE::JSON_KEY::m_fRadius);
	//if (jVal.isMember(strKey))
	//	m_fRadius = jVal[strKey].asFloat();
	//else
	//	return false;

	return true;
}


void Collider2D_Circle::UpdateSimpleCollider(Vec4& _vSimpleCollLBRTPos)
{
	const Vec2& v2Pos = GetCenterPos().XY();

	//반지름은 X값만을 사용.
	float fRadius = GetCollSize().x;

	//타원은 지원하지 않음
	Vec2 LBRT(fRadius, fRadius);

	_vSimpleCollLBRTPos = Vec4(v2Pos - LBRT, v2Pos + LBRT);
}

void Collider2D_Circle::DebugRender()
{
	const Transform& pTransform = Transform();

	const Matrix& matOffset = Matrix::CreateTranslation(GetCenterPos());

	//원을 생성
	const Matrix& matSize = Matrix::CreateScale(GetRadius() * 2.f);

	tDebugShapeInfo Info = {};
	Info.eShapeType = (int)eDEBUGSHAPE_TYPE::CIRCLE;

	Info.matWorld = matSize * matOffset;

	//충돌 중인 물체가 있을 경우 빨강, 아닐 경우 초록
	Info.vColor = 0 == GetCollisionCount() ? Vec4(0.f, 1.f, 0.f, 1.f) : Vec4(1.f, 0.f, 0.f, 1.f);
	RenderMgr::GetInst()->AddDebugShapeRender(Info);
}
