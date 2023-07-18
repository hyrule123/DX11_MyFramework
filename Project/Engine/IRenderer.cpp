#include "pch.h"
#include "IRenderer.h"

//출력을 위한 클래스
#include "cMesh.h"
#include "cMaterial.h"

#include "jsoncpp.h"

#include "cResMgr.h"

#include "cDevice.h"
#include "cConstBuffer.h"

#include "cGameObject.h"

IRenderer::IRenderer()
	: IComponent(eCOMPONENT_TYPE::RENDERER)
	, m_bIsDynamicMode()
{
}

IRenderer::IRenderer(const IRenderer& _other)
	: IComponent(_other)
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

IRenderer::~IRenderer()
{
}

bool IRenderer::SaveJson(Json::Value* _pJVal)
{
	if (nullptr == _pJVal)
		return false;
	else if (false == IComponent::SaveJson(_pJVal))
		return false;

	if (false == IsRenderReady())
	{
		ERROR_MESSAGE("Render IComponent is not prepared for render!!");
		return false;
	}

	Json::Value& jVal = *_pJVal;

	jVal[string(RES_INFO::PREFAB::COMPONENT::RENDER_COMP::JSON_KEY::strKeyMesh)] = m_pMesh->GetKey();

	jVal[string(RES_INFO::PREFAB::COMPONENT::RENDER_COMP::JSON_KEY::strKeyMtrl)] = m_pSharedMtrl->GetKey();

	//재질은 파일로 따로 저장
	m_pSharedMtrl->Save(m_pSharedMtrl->GetKey());

	return true;
}

bool IRenderer::LoadJson(Json::Value* _pJVal)
{
	if (nullptr == _pJVal)
		return false;
	else if (false == IComponent::LoadJson(_pJVal))
		return false;

	const Json::Value& jVal = *_pJVal;

	{
		string strKey = string(RES_INFO::PREFAB::COMPONENT::RENDER_COMP::JSON_KEY::strKeyMesh);
		if (jVal.isMember(strKey))
		{
			m_pMesh = cResMgr::GetInst()->Load<cMesh>(jVal[strKey].asString());

			if (nullptr == m_pMesh)
			{
				ERROR_MESSAGE("Failed to load cMesh!!");
				return false;
			}
		}
		else return false;
	}

	{
		string strKey = string(RES_INFO::PREFAB::COMPONENT::RENDER_COMP::JSON_KEY::strKeyMtrl);
		if (jVal.isMember(strKey))
		{
			Ptr<cMaterial> pMtrl = cResMgr::GetInst()->Load<cMaterial>(jVal[strKey].asString());

			if (nullptr == pMtrl)
			{
				ERROR_MESSAGE("Failed to load cMaterial!!");
				return false;
			}

			SetMaterial(pMtrl);
		}
		else return false;
	}

	return true;
}

void IRenderer::BindMtrlScalarDataToCBuffer()
{
	static cConstBuffer* pCBuffer = cDevice::GetInst()->GetConstBuffer(REGISLOT_b_CBUFFER_MTRL_SCALAR);
	const tMtrlScalarData& pMtrlData = GetOwner()->GetMtrlScalarData();
	pCBuffer->UploadData((void*)&pMtrlData);
	pCBuffer->BindBuffer();
}

Ptr<cMaterial> IRenderer::GetDynamicMaterial()
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

const tMtrlScalarData& IRenderer::GetMtrlScalarData() const
{
	return GetOwner()->GetMtrlScalarData();
}


void IRenderer::SetCamIdx(eCAMERA_INDEX _eCamIdx)
{
	cGameObject* pOwner = GetOwner();
	if (nullptr != pOwner)
		pOwner->SetMtrlScalarParam(MTRL_SCALAR_INT_CAMIDX, &_eCamIdx);
}

