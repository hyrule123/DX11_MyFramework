#include "pch.h"
#include "CCollider.h"

#include "CScriptHolder.h"
#include "CScript.h"

#include "CTransform.h"

CCollider::CCollider(eCOMPONENT_TYPE _ComType, eDIMENSION_TYPE _eDim)
	: CComponent(_ComType)
	, m_eCollDimension(_eDim)
	, m_iCollisionCount()
	, m_v3Size(Vec3(100.f))
	, m_bFollowTransformSize(true)
	, m_bCollPosUpdated(0x02)
	, m_bCollSizeUpdated(0x02)
{
}

CCollider::~CCollider()
{
}


//자신과 상대방의 begincollision 호출
void CCollider::BeginCollision(CCollider* _other, const Vec3& _v3HitPoint)
{
	++m_iCollisionCount;
	_other->AddCollisionCount();

	CScriptHolder* pSH = GetOwner()->ScriptHolder();
	if (nullptr != pSH)
		pSH->BeginColiision(_other, _v3HitPoint);
	

	pSH = _other->GetOwner()->ScriptHolder();
	if (nullptr != pSH)
		pSH->BeginColiision(this, _v3HitPoint);
}

void CCollider::OnCollision(CCollider* _other, const Vec3& _v3HitPoint)
{
	CScriptHolder* pSH = GetOwner()->ScriptHolder();
	if (nullptr != pSH)
		pSH->OnCollision(_other, _v3HitPoint);


	pSH = _other->GetOwner()->ScriptHolder();
	if (nullptr != pSH)
		pSH->OnCollision(this, _v3HitPoint);
}

void CCollider::EndCollision(CCollider* _other)
{
	--m_iCollisionCount;
	_other->SubCollisionCount();

	CScriptHolder* pSH = GetOwner()->ScriptHolder();
	if (nullptr != pSH)
		pSH->EndCollision(_other);


	pSH = _other->GetOwner()->ScriptHolder();
	if (nullptr != pSH)
		pSH->EndCollision(this);
}

void CCollider::finaltick()
{
	m_bCollPosUpdated >>= 1;
	m_bCollSizeUpdated >>= 1;

	CTransform* pTransform = Transform();

	//트랜스폼의 위치정보가 변경되었을 경우 중심점을 새로 계산(위치는 무조건 Transform을 따라감.)
	if (true == pTransform->IsUpdated() || isCollPosUpdated())
	{
		m_bCollPosUpdated |= 0x01;

		//자신의 중심 위치를 구한다.
		const Vec3& WorldPos = Transform()->GetWorldPos();

		m_v3CenterPos = WorldPos + m_v3OffsetPos;
	}

	//트랜스폼의 사이즈를 따라가고, 트랜스폼의 사이즈가 업데이트 되었을 경우 자신의 사이즈를 업데이트
	if (m_bFollowTransformSize && pTransform->GetSizeUpdated())
	{
		m_bCollSizeUpdated |= 0x01;
		m_v3Size = pTransform->GetWorldSize();
	}

	//충돌체를 업데이트 해야 하는 경우 UpdateCollider() 함수 호출.
	if (isCollPosUpdated() || isCollSizeUpdated())
		UpdateCollider();
}


