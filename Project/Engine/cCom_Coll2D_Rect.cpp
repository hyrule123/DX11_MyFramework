#include "pch.h"
#include "cCom_Coll2D_Rect.h"

#include "cTransform.h"

#include "S_H_Debug.hlsli"
#include "cRenderMgr.h"

#include "strKey_Default.h"
#include "jsoncpp.h"

cCom_Coll2D_Rect::cCom_Coll2D_Rect()
	: ICollider2D(eCOLLIDER_TYPE_2D::RECT)
{
}

cCom_Coll2D_Rect::cCom_Coll2D_Rect(eCOLLIDER_TYPE_2D _Type)
	: ICollider2D(_Type)
{
}


cCom_Coll2D_Rect::~cCom_Coll2D_Rect()
{
}

bool cCom_Coll2D_Rect::SaveJson(Json::Value* _pJVal)
{
	if (nullptr == _pJVal)
		return false;
	else if (false == ICollider2D::SaveJson(_pJVal))
		return false;

	Json::Value& jVal = *_pJVal;

	/*jVal[string(RES_INFO::PREFAB::COMPONENT::COLLIDER2D::RECT::JSON_KEY::m_v2RectSize)] = Pack_v2_i64(m_v2RectSize).i64;*/

	return true;
}

bool cCom_Coll2D_Rect::LoadJson(Json::Value* _pJVal)
{
	if (nullptr == _pJVal)
		return false;
	else if (false == ICollider2D::LoadJson(_pJVal))
		return false;

	const Json::Value& jVal = *_pJVal;

	//string strKey = string(RES_INFO::PREFAB::COMPONENT::COLLIDER2D::RECT::JSON_KEY::m_v2RectSize);
	//if (jVal.isMember(strKey))
	//{
	//	m_v2RectSize = Pack_v2_i64(jVal[strKey].asInt64()).v2;
	//}
	//else return false;


	return true;
}



void cCom_Coll2D_Rect::UpdateCollider()
{
	ICollider2D::UpdateCollider();
}

void cCom_Coll2D_Rect::UpdateSimplecCollider(Vec4& _vSimpleCollLBRTPos)
{
	const Vec2& v2Center = GetCenterPos().XY();
	_vSimpleCollLBRTPos = Vec4(v2Center, v2Center);
	const Vec2& halfLen = GetCollSize().XY() * 0.5f;

	//LB쪽으로는 빼주고, RT쪽으로는 더해준다. 계산 끝
	_vSimpleCollLBRTPos += Vec4(-halfLen, halfLen);
}

void cCom_Coll2D_Rect::DebugRender()
{
	//간이충돌체를 그대로 써서 만들어주면 됨
	const Vec4& LBRT = GetSimplecCollider();

	const Vec3& v3Size = GetCollSize();

	const Matrix& matScale = Matrix::CreateScale(GetCollSize());

	const Matrix& matPos = Matrix::CreateTranslation(GetCenterPos());

	tDebugShapeInfo Info = {};
	Info.eShapeType = (int)eDEBUGSHAPE_TYPE::RECT;
	Info.matWorld = matScale * matPos;

	//충돌 중인 물체가 있을 경우 빨강, 아닐 경우 초록
	Info.vColor = 0 == GetCollisionCount() ? Vec4(0.f, 1.f, 0.f, 1.f) : Vec4(1.f, 0.f, 0.f, 1.f);
	cRenderMgr::GetInst()->AddDebugShapeRender(Info);
}
