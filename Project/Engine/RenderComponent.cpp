#include "pch.h"
#include "RenderComponent.h"

//출력을 위한 클래스
#include "Mesh.h"
#include "Material.h"

#include "jsoncpp.h"

#include "ResMgr.h"

#include "Device.h"
#include "cConstBuffer.h"

RenderComponent::RenderComponent(eCOMPONENT_TYPE _type)
	: Component(_type)
	, m_bIsDynamicMode()
{
}

RenderComponent::RenderComponent(const RenderComponent& _other)
	: Component(_other)
	, m_pMesh(_other.m_pMesh)
	, m_pSharedMtrl(_other.m_pSharedMtrl)
	, m_bIsDynamicMode(_other.m_bIsDynamicMode)
{
	assert(nullptr != m_pSharedMtrl);

	if (m_bIsDynamicMode)
	{
		m_pDynamicMtrl = _other.m_pDynamicMtrl->Clone();
		GetDynamicMaterial();
	}
	else
		GetSharedMaterial();
}

RenderComponent::~RenderComponent()
{
}

bool RenderComponent::SaveJson(Json::Value* _pJVal)
{
	if (nullptr == _pJVal)
		return false;
	else if (false == Component::SaveJson(_pJVal))
		return false;

	if (false == IsRenderReady())
	{
		ERROR_MESSAGE("Render Component is not prepared for render!!");
		return false;
	}

	Json::Value& jVal = *_pJVal;

	jVal[string(RES_INFO::PREFAB::COMPONENT::RENDER_COMP::JSON_KEY::strKeyMesh)] = m_pMesh->GetKey();

	jVal[string(RES_INFO::PREFAB::COMPONENT::RENDER_COMP::JSON_KEY::strKeyMtrl)] = m_pSharedMtrl->GetKey();

	//재질은 파일로 따로 저장
	m_pSharedMtrl->Save(m_pSharedMtrl->GetKey());

	return true;
}

bool RenderComponent::LoadJson(Json::Value* _pJVal)
{
	if (nullptr == _pJVal)
		return false;
	else if (false == Component::LoadJson(_pJVal))
		return false;

	const Json::Value& jVal = *_pJVal;

	{
		string strKey = string(RES_INFO::PREFAB::COMPONENT::RENDER_COMP::JSON_KEY::strKeyMesh);
		if (jVal.isMember(strKey))
		{
			m_pMesh = ResMgr::GetInst()->Load<Mesh>(jVal[strKey].asString());

			if (nullptr == m_pMesh)
			{
				ERROR_MESSAGE("Failed to load Mesh!!");
				return false;
			}
		}
		else return false;
	}

	{
		string strKey = string(RES_INFO::PREFAB::COMPONENT::RENDER_COMP::JSON_KEY::strKeyMtrl);
		if (jVal.isMember(strKey))
		{
			Ptr<Material> pMtrl = ResMgr::GetInst()->Load<Material>(jVal[strKey].asString());

			if (nullptr == pMtrl)
			{
				ERROR_MESSAGE("Failed to load Material!!");
				return false;
			}

			SetMaterial(pMtrl);
		}
		else return false;
	}

	return true;
}

void RenderComponent::BindMtrlScalarDataToCBuffer()
{
	static cConstBuffer* pCBuffer = Device::GetInst()->GetConstBuffer(REGISLOT_b_CBUFFER_MTRL_SCALAR);
	const tMtrlScalarData& pMtrlData = GetOwner()->GetMtrlScalarData();
	pCBuffer->UploadData((void*)&pMtrlData);
	pCBuffer->BindBuffer();
}

Ptr<Material> RenderComponent::GetDynamicMaterial()
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
	m_bIsDynamicMode = true;
	return m_pCurrentMtrl;
}


void RenderComponent::SetCamIdx(eCAMERA_INDEX _eCamIdx)
{
	GameObject* pOwner = GetOwner();
	if (nullptr != pOwner)
		pOwner->SetMtrlScalarParam(MTRL_SCALAR_INT_CAMIDX, &_eCamIdx);
}

