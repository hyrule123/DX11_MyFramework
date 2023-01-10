#include "pch.h"
#include "CRenderComponent.h"

//����� ���� Ŭ����
#include "CMesh.h"
#include "CMaterial.h"


CRenderComponent::CRenderComponent(eCOMPONENT_TYPE _type):
	CComponent(_type)
{
}

CRenderComponent::CRenderComponent(const CRenderComponent& _other)
	: CComponent(_other)
	, m_pMesh(_other.m_pMesh)
{
	//������ ��� ������ �ʿ��� ��� ���� ����ü�� �ʿ��ϹǷ� ���� �����ڸ� ȣ�����־�� �Ѵ�.
	m_pMtrl = _other.m_pMtrl.Get()->Clone();
}

CRenderComponent::~CRenderComponent()
{
}
