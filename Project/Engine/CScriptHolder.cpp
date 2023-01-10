#include "pch.h"
#include "CScriptHolder.h"

#include "CScript.h"

bool CScriptHolder::AddScript(CScript* _pScript)
{
	if (nullptr == _pScript)
		return false;

	std::type_index type = _pScript->GetTypeIndex();

	//��ũ��Ʈ �ߺ� ����
	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (type == m_vecScript[i]->GetTypeIndex())
		{
			delete _pScript;
			return false;
		}
	}

	m_vecScript.push_back(_pScript);
	_pScript->SetOwner(GetOwner());

	return true;
}

void CScriptHolder::init()
{
	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecScript[i]->init();
	}
}

void CScriptHolder::SetOwner(CGameObject* _pOwner)
{
	CComponent::SetOwner(_pOwner);

	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecScript[i]->SetOwner(_pOwner);
	}
}

void CScriptHolder::tick()
{
	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecScript[i]->tick();
	}
}

CScriptHolder::CScriptHolder()
	: CComponent(eCOMPONENT_TYPE::eCOMPONENT_SCRIPT_HOLDER)
{
}

CScriptHolder::CScriptHolder(const CScriptHolder& _other)
	: CComponent(_other)
{
	size_t size = _other.m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecScript.push_back(_other.m_vecScript[i]->Clone());

		//���� �����ڿ��� ���� ������ ������ ������ �Էµ��� �ʾ����Ƿ� 
		//SetOwner���� ó������� �Ѵ�.
	}
}

CScriptHolder::~CScriptHolder()
{
	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (nullptr != m_vecScript[i])
			delete m_vecScript[i];
	}
}
