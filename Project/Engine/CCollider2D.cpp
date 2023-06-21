#include "pch.h"

#include "CCollisionMgr.h"

#include "CCollider2D.h"

#include "CCollider2D_Rect.h"
#include "CCollider2D_Circle.h"
#include "CCollider2D_OBB.h"

#include "CRenderMgr.h"

#include "CTransform.h"

#include "strKey_Default.h"
#include "jsoncpp.h"

CCollider2D::CCollider2D(eCOLLIDER_TYPE_2D _eColType)
	: CCollider(eCOMPONENT_TYPE::COLLIDER2D, eDIMENSION_TYPE::_2D)
	, m_eColType(_eColType)
	, m_vecGridIdxInfo{}
{
}


CCollider2D::~CCollider2D()
{
}

bool CCollider2D::SaveJson(Json::Value* _pJVal)
{
	if (nullptr == _pJVal)
		return false;
	else if (false == CCollider::SaveJson(_pJVal))
		return false;

	Json::Value& jVal = *_pJVal;

	string strKey = string(RES_INFO::PREFAB::COMPONENT::COLLIDER2D::JSON_KEY::m_eColType);
	jVal[strKey] = (int)m_eColType;


	return true;
}

bool CCollider2D::LoadJson(Json::Value* _pJVal)
{
	if (nullptr == _pJVal)
		return false;
	else if (false == CCollider::LoadJson(_pJVal))
		return false;

	const Json::Value& jVal = *_pJVal;

	//콜라이더 타입이 일치할 경우에만 true 반환
	string strKey = string(RES_INFO::PREFAB::COMPONENT::COLLIDER2D::JSON_KEY::m_eColType);
	if (jVal.isMember(strKey))
	{
		if ((int)m_eColType == jVal[strKey].asInt())
			return true;
	}

	ERROR_MESSAGE("Collider Type Not Matching!!");
	return false;
}

void CCollider2D::finaltick()
{
	CCollider::finaltick();

	//계산된 위치 정보를 공간분할 그리드에 추가한다.
	CCollisionMgr::GetInst()->AddCollider2D(this, m_vecGridIdxInfo);

	//에디터 카메라 모드일만 디버그
	if(true == CRenderMgr::GetInst()->IsEditorCamMode())
		DebugRender();
}

void CCollider2D::UpdateCollider()
{
	//간이 충돌체 정보만 갱신한다. 기본 설정은 Transform에서 변의 길이를 받아와서 정사각형 형태로 생성함.
	UpdateSimpleCollider(m_v4SimpleCollLBRTPos);

	//위치 LBRT 정보를 전달해서 공간분할 그리드 상에서의 인덱스 정보를 받아온다.
	CCollisionMgr::GetInst()->CalcSimpleCollGrid2D(this, m_v4SimpleCollLBRTPos, m_vecGridIdxInfo);
}

//void CCollider2D::UpdateSimpleCollider(Vec4& _vSimpleCollLBRTPos)
//{
//	const CTransform& pTransform = Transform();
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



