#include "pch.h"
#include "CCollider2D_Rect.h"

#include "CTransform.h"

#include "S_0_H_Debug.hlsli"
#include "CRenderMgr.h"

CCollider2D_Rect::CCollider2D_Rect()
	: CCollider2D(eCOLLIDER_TYPE_2D::RECT)
{
}

CCollider2D_Rect::CCollider2D_Rect(eCOLLIDER_TYPE_2D _Type)
	: CCollider2D(_Type)
{
}


CCollider2D_Rect::~CCollider2D_Rect()
{
}

void CCollider2D_Rect::SetSCBuildingSize(UINT _uNumMegatileX, UINT _uNumMegatileY, const Vec4& _v4LRTBOffset)
{
	enum LBRT { L, R, T, B };
	//메가타일에서의 사이즈
	Vec2 v2SizeInPixel = Vec2(32.f * (float)_uNumMegatileX, 32.f * (float)_uNumMegatileY);
	
	m_v2RectSize = Vec2 (v2SizeInPixel.x - (_v4LRTBOffset[L] + _v4LRTBOffset[R]), v2SizeInPixel.y - (_v4LRTBOffset[B] + _v4LRTBOffset[T]));
	Vec3 test = Vec3(_v4LRTBOffset[L] - _v4LRTBOffset[R], _v4LRTBOffset[B] - _v4LRTBOffset[T], 0.f);
	SetOffsetPos(Vec3(_v4LRTBOffset[L] - _v4LRTBOffset[R], _v4LRTBOffset[B] - _v4LRTBOffset[T], 0.f));
}

void CCollider2D_Rect::UpdateCollider()
{
	//할 거 없음

	//CTransform* pTransform = Transform();
	//if (nullptr == pTransform)
	//	return;

	////회전이 적용되지 않는 충돌체이므로 Scale만 받아 온다.

	//pTransform->GetWorldScale()

	
}

void CCollider2D_Rect::UpdateSimpleCollider(Vec4& _vSimpleCollLBRTPos)
{
	CTransform* pTransform = Transform();
	assert(nullptr != pTransform);

	_vSimpleCollLBRTPos = Vec4(GetCenterPos(), GetCenterPos());
	const Vec2 halfLen = m_v2RectSize * 0.5f;

	//LB쪽으로는 빼주고, RT쪽으로는 더해준다. 계산 끝
	_vSimpleCollLBRTPos += Vec4(-halfLen, halfLen);
	
	//Offset이 반영된 중심 좌표는 CCollider2D에서 계산되므로 여기서는 그 값을 가져다 쓰기만 하면 된다.

	////대입하고
	//_vSimpleCollLBRTPos = m_v4LBRTLength;

	////크기(Scale) 반영하고 + L과 B는 음수로 바꿔 줌
	//const Vec3& Scale = pTransform->GetWorldScale();
	//_vSimpleCollLBRTPos *= Vec4(-Scale.x, -Scale.y, Scale.x, Scale.y);

	////가운데 위치만큼 이동시키면 끝
	//const Vec2& CenterPos = GetCenterPos();
	//_vSimpleCollLBRTPos += Vec4(CenterPos.x, CenterPos.y, CenterPos.x, CenterPos.y);
}

void CCollider2D_Rect::DebugRender()
{
	//간이충돌체를 그대로 써서 만들어주면 됨
	const Vec4& LBRT = GetSimpleCollider();
	const Matrix& matScale = Matrix::CreateScale(m_v2RectSize.x, m_v2RectSize.y, 1.f);

	const Matrix& matPos = Matrix::CreateTranslation(Vec3(GetCenterPos(), 0.f));

	tDebugShapeInfo Info = {};
	Info.eShapeType = (int)eDEBUGSHAPE_TYPE::RECT;
	Info.matWorld = matScale * matPos;

	//충돌 중인 물체가 있을 경우 빨강, 아닐 경우 초록
	Info.vColor = 0 == GetCollisionCount() ? Vec4(0.f, 1.f, 0.f, 1.f) : Vec4(1.f, 0.f, 0.f, 1.f);
	CRenderMgr::GetInst()->AddDebugShapeRender(Info);
}
