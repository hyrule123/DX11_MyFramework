#include "pch.h"

#include "cCollisionMgr.h"

#include "ICollider2D.h"

#include "cCom_Coll2D_Rect.h"
#include "cCom_Coll2D_Circle.h"
#include "cCom_Coll2D_OBB.h"

#include "cRenderMgr.h"

#include "cTransform.h"

#include "strKey_Default.h"
#include "jsoncpp.h"

ICollider2D::ICollider2D(eCOLLIDER_TYPE_2D _eColType)
	: ICollider(eCOMPONENT_TYPE::COLLIDER2D, eDIMENSION_TYPE::_2D)
	, m_eColType(_eColType)
	, m_vecGridIdxInfo{}
{
}


ICollider2D::~ICollider2D()
{
}

bool ICollider2D::SaveJson(Json::Value* _pJVal)
{
	if (nullptr == _pJVal)
		return false;
	else if (false == ICollider::SaveJson(_pJVal))
		return false;

	Json::Value& jVal = *_pJVal;

	string strKey = string(RES_INFO::PREFAB::COMPONENT::COLLIDER2D::JSON_KEY::m_eColType);
	jVal[strKey] = (int)m_eColType;


	return true;
}

bool ICollider2D::LoadJson(Json::Value* _pJVal)
{
	if (nullptr == _pJVal)
		return false;
	else if (false == ICollider::LoadJson(_pJVal))
		return false;

	const Json::Value& jVal = *_pJVal;

	//콜라이더 타입이 일치할 경우에만 true 반환
	string strKey = string(RES_INFO::PREFAB::COMPONENT::COLLIDER2D::JSON_KEY::m_eColType);
	if (jVal.isMember(strKey))
	{
		if ((int)m_eColType == jVal[strKey].asInt())
			return true;
	}

	ERROR_MESSAGE("ICollider Type Not Matching!!");
	return false;
}

void ICollider2D::finaltick()
{
	ICollider::finaltick();

	//계산된 위치 정보를 공간분할 그리드에 추가한다.
	cCollisionMgr::GetInst()->AddcCollider2D(this, m_vecGridIdxInfo);

	//에디터 카메라 모드일만 디버그
	if(true == cRenderMgr::GetInst()->IsEditorCamMode())
		DebugRender();
}

void ICollider2D::UpdateCollider()
{
	//간이 충돌체 정보만 갱신한다. 기본 설정은 cTransform에서 변의 길이를 받아와서 정사각형 형태로 생성함.
	UpdateSimplecCollider(m_v4SimpleCollLBRTPos);

	//위치 LBRT 정보를 전달해서 공간분할 그리드 상에서의 인덱스 정보를 받아온다.
	cCollisionMgr::GetInst()->CalcSimpleCollGrid2D(this, m_v4SimpleCollLBRTPos, m_vecGridIdxInfo);
}

//void ICollider2D::UpdateSimplecCollider(Vec4& _vSimpleCollLBRTPos)
//{
//	const cTransform& pTransform = Transform();
//
//	if (nullptr != pTransform)
//	{
//		//이 값은 트랜스폼의 크기와 사이즈가 모두 적용된 값임.
//		float Sidelen = pTransform->GetAABBSideLen();
//
//		//LB
//		_vSimpleCollLBRTPos.x = m_v2CenterPos.x - Sidelen;
//		_vSimpleCollLBRTPos.y = m_v2CenterPos.y - Sidelen;
//
//		//RT
//		Sidelen *= 2.f;
//		_vSimpleCollLBRTPos.z = _vSimpleCollLBRTPos.x + Sidelen;
//		_vSimpleCollLBRTPos.w = _vSimpleCollLBRTPos.y + Sidelen;
//	}
//}



