#include "pch.h"
#include "CGameObject.h"

#include "CComponent.h"
#include "CMeshRender.h"


CGameObject::CGameObject()
	: m_arrCom{}
{
}

CGameObject::~CGameObject()
{
	/*for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			delete m_arrCom[i];
	}*/

	Safe_Del_Array(m_arrCom);
}

void CGameObject::tick()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->tick();
	}
}

void CGameObject::finaltick()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::SCRIPT; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->finaltick();
	}
}

void CGameObject::render()
{
	if (nullptr == MeshRender())
		return;

	MeshRender()->render();
}

void CGameObject::AddComponent(CComponent* _Component)
{
	// �̹� �����ϰ� �ִ� ������Ʈ�� ���
	assert(!m_arrCom[(UINT)_Component->GetType()]);

	_Component->m_pOwner = this;
	m_arrCom[(UINT)_Component->GetType()] = _Component;
}