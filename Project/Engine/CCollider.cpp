#include "pch.h"
#include "CCollider.h"

#include "CScriptHolder.h"
#include "CScript.h"

CCollider::CCollider(eCOMPONENT_TYPE _ComType, eDIMENSION_TYPE _eDim)
	: CComponent(_ComType)
	, m_eCollDimension(_eDim)
	
	, m_iCollisionCount()

	//, m_vOffsetScale(Vec3::One)
	//, m_bFixSize()
	//, m_bNeedPosUpdate(true)
	//, m_bNeedRotUpdate(true)
	//, m_bNeedScaleSizeUpdate(true)
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

