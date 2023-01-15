#include "pch.h"
#include "CCollider.h"

#include "CScriptHolder.h"
#include "CScript.h"

CCollider::CCollider(eCOMPONENT_TYPE _ComType, eCOLLIDER_TYPE _ColType)
	: CComponent(_ComType)
	, m_eColType(_ColType)
{
}

CCollider::~CCollider()
{
}

void CCollider::BeginCollision(CCollider* _other)
{
	_other->BeginCollision(this);

	CScriptHolder* pSH = GetOwner()->ScriptHolder();
	if (nullptr == pSH)
		return;

	const vector<CScript*>& vecScript = pSH->GetScripts();
	size_t size = vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		vecScript[i]->BeginCollision(_other);
	}
}

void CCollider::OnCollision(CCollider* _other)
{
	_other->OnCollision(this);

	CScriptHolder* pSH = GetOwner()->ScriptHolder();
	if (nullptr == pSH)
		return;

	const vector<CScript*>& vecScript = pSH->GetScripts();
	size_t size = vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		vecScript[i]->OnCollision(_other);
	}
}

void CCollider::EndCollision(CCollider* _other)
{
	_other->EndCollision(this);

	CScriptHolder* pSH = GetOwner()->ScriptHolder();
	if (nullptr == pSH)
		return;

	const vector<CScript*>& vecScript = pSH->GetScripts();
	size_t size = vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		vecScript[i]->EndCollision(_other);
	}
}

//
void CCollider::finaltick()
{

}
