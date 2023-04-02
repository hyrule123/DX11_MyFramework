#include "pch.h"
#include "CCollider2D_Circle.h"

#include "CTransform.h"
#include "S_0_H_Debug.hlsli"
#include "CRenderMgr.h"

#include "strKeyDefault.h"

#include "jsoncpp.h"

CCollider2D_Circle::CCollider2D_Circle()
	: CCollider2D(eCOLLIDER_TYPE_2D::CIRCLE)
	, m_fRadius(100.f)
{
}

CCollider2D_Circle::CCollider2D_Circle(const CCollider2D_Circle& _other)
	: CCollider2D(_other)
	, m_fRadius(_other.m_fRadius)
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

	jVal[string(RES_INFO::PREFAB::COMPONENT::COLLIDER2D::CIRCLE::JSON_KEY::m_fRadius)] = m_fRadius;

	return true;
}

bool CCollider2D_Circle::LoadJson(Json::Value* _pJVal)
{
	if (nullptr == _pJVal)
		return false;
	else if (false == CCollider2D::LoadJson(_pJVal))
		return false;

	Json::Value& jVal = *_pJVal;

	string strKey = string(RES_INFO::PREFAB::COMPONENT::COLLIDER2D::CIRCLE::JSON_KEY::m_fRadius);
	if (jVal.isMember(strKey))
		m_fRadius = jVal[strKey].asFloat();
	else
		return false;

	return true;
}

void CCollider2D_Circle::UpdateCollider()
{
}

void CCollider2D_Circle::UpdateSimpleCollider(Vec4& _vSimpleCollLBRTPos)
{
	const Vec2& v2Pos = GetCenterPos();

	Vec2 LBRT(m_fRadius, m_fRadius);

	_vSimpleCollLBRTPos = Vec4(v2Pos - LBRT, v2Pos + LBRT);
}

void CCollider2D_Circle::DebugRender()
{
	CTransform* pTransform = Transform();

	const Matrix& matWorld = pTransform->GetWorldMatWithoutSize();

	const Matrix& matOffset = Matrix::CreateTranslation(GetOffsetPos());

	//20 사이즈의 작은 원을 생성
	const static Matrix matSize = Matrix::CreateScale(m_fRadius);

	tDebugShapeInfo Info = {};
	Info.eShapeType = (int)eDEBUGSHAPE_TYPE::CIRCLE;

	Info.matWorld = matSize * matOffset * matWorld;

	//충돌 중인 물체가 있을 경우 빨강, 아닐 경우 초록
	Info.vColor = 0 == GetCollisionCount() ? Vec4(0.f, 1.f, 0.f, 1.f) : Vec4(1.f, 0.f, 0.f, 1.f);
	CRenderMgr::GetInst()->AddDebugShapeRender(Info);
}
