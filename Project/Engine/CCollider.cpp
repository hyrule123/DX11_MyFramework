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
void CCollider::BeginCollision(CCollider* _other)
{
	++m_iCollisionCount;
	_other->AddCollisionCount();

	CScriptHolder* pSH = GetOwner()->ScriptHolder();
	if (nullptr != pSH)
		pSH->BeginColiision(_other);
	

	pSH = _other->GetOwner()->ScriptHolder();
	if (nullptr != pSH)
		pSH->BeginColiision(this);
}

void CCollider::OnCollision(CCollider* _other)
{
	CScriptHolder* pSH = GetOwner()->ScriptHolder();
	if (nullptr != pSH)
		pSH->OnCollision(_other);


	pSH = _other->GetOwner()->ScriptHolder();
	if (nullptr != pSH)
		pSH->OnCollision(this);
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

