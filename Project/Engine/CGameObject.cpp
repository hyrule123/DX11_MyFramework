#include "pch.h"
#include "CGameObject.h"

#include "CComponent.h"
#include "CMeshRender.h"


CGameObject::CGameObject()
	: m_arrCom{}
	, m_initalized(false)
	, m_RenderCom()
{
}

CGameObject::~CGameObject()
{
	/*for (UINT i = 0; i < (UINT)eCOMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			delete m_arrCom[i];
	}*/

	Safe_Del_Array(m_arrCom);
}

void CGameObject::init()
{
	m_initalized = true;

	for (UINT i = 0; i < (UINT)eCOMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->init();
	}
}

void CGameObject::tick()
{
	for (UINT i = 0; i < (UINT)eCOMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->tick();
	}
}

void CGameObject::finaltick()
{
	for (UINT i = 0; i < (UINT)eCOMPONENT_TYPE::SCRIPT; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->finaltick();
	}
}

void CGameObject::render()
{
	if (nullptr == m_RenderCom)
		return;

	m_RenderCom->render();
}

void CGameObject::AddComponent(CComponent* _Component)
{
	UINT ComType = (UINT)_Component->GetType();

	// nullptr�� �ƴ� ��� assert
	assert(nullptr == m_arrCom[ComType]);


	if (
		(UINT)g_RenderComponentStart <= ComType
		&&
		(UINT)g_RenderComponentEnd > ComType
		)
	{
		//m_RenderCom�� �ϳ� �̻��� Render ������Ʈ�� �� ���� ��� ���� �߻���Ŵ.
		assert(nullptr == m_RenderCom);
		m_RenderCom = static_cast<CRenderComponent*>(_Component);
	}

	_Component->m_pOwner = this;
	m_arrCom[(UINT)_Component->GetType()] = _Component;


	//�̹� �۵����� ��� �ٷ� init() ȣ��
	if(m_initalized)
		_Component->init();
}