#include "pch.h"
#include "CRenderComponent.h"

//출력을 위한 클래스
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
	//재질의 경우 개별로 필요한 상수 버퍼 구조체가 필요하므로 복사 생성자를 호출해주어야 한다.
	m_pMtrl = _other.m_pMtrl.Get()->Clone();
}

CRenderComponent::~CRenderComponent()
{
}
