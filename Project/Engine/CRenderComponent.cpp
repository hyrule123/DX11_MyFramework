#include "pch.h"
#include "CRenderComponent.h"

//출력을 위한 클래스
#include "CMesh.h"
#include "CMaterial.h"


CRenderComponent::CRenderComponent(eCOMPONENT_TYPE _type)
	: CComponent(_type)
{
}

CRenderComponent::CRenderComponent(const CRenderComponent& _other)
	: CComponent(_other)
	, m_pMesh(_other.m_pMesh)
	, m_pSharedMtrl(_other.m_pSharedMtrl)
{
}

CRenderComponent::~CRenderComponent()
{
}

Ptr<CMaterial> CRenderComponent::GetDynamicMaterial()
{
	//원본 재질이 없을 경우 nullptr을 반환한다.
	if (nullptr == m_pSharedMtrl)
	{
		m_pCurrentMtrl = nullptr;
		return nullptr;
	}

	//동적 재질 최초 요청시 clone해서 복사 후 돌려준다.
	if (nullptr == m_pDynamicMtrl)
	{
		m_pDynamicMtrl = m_pSharedMtrl->Clone();
	}

	m_pCurrentMtrl = m_pDynamicMtrl;
	return m_pCurrentMtrl;
}

void CRenderComponent::SetMtrlScalarParam(const tMtrlScalarData& _tMtrlScalarData)
{
	m_pCurrentMtrl->AddMtrlScalarData(_tMtrlScalarData);
}

void CRenderComponent::SetCamIdx(eCAMERA_INDEX _eCamIdx)
{
	CGameObject* pOwner = GetOwner();
	if (nullptr != pOwner)
		pOwner->SetMtrlScalarParam(MTRL_SCALAR_INT_CAMIDX, &_eCamIdx);
}

