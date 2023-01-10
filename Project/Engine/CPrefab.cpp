#include "pch.h"
#include "CPrefab.h"

#include "CGameObject.h"

void CPrefab::RegisterPrefab(CGameObject* _pPrefab)
{
	//Prefab�� ���̾ ��ϵǾ� ����Ǵ� �뵵�� �ƴ�.
	assert(0 > _pPrefab->GetLayer());

	m_pPrefab = _pPrefab;
}

CGameObject* CPrefab::Instantiate()
{
	assert(m_pPrefab != nullptr);
	return m_pPrefab->Clone();
}

CPrefab::CPrefab() 
	: CRes(eRES_TYPE::PREFAB)
	, m_pPrefab()
{
}

CPrefab::~CPrefab()
{
	delete m_pPrefab;
}