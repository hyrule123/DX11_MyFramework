#include "pch.h"
#include "CMaterial.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CStructBuffer.h"

#include "jsoncpp.h"

#include "CResMgr.h"





CMaterial::CMaterial()
	: CRes(eRES_TYPE::MATERIAL)
	, m_MtrlTex{}
	, m_arrTex{}
	, m_bIsDynamic()
{
}

CMaterial::CMaterial(const CMaterial& _Clone)
	: CRes(_Clone)
	, m_pShader(_Clone.m_pShader)
	, m_MtrlTex(_Clone.m_MtrlTex)
	, m_bIsDynamic(true)
{
	for (int i = 0; i < (int)eMTRLDATA_PARAM_TEX::_END; ++i)
	{
		m_arrTex[i] = _Clone.m_arrTex[i];
	}
}



CMaterial::~CMaterial()
{
	//상수버퍼는 CDevice에서 관리하고, 구조화버퍼는 여기서 관리하므로 구조화버퍼만 제거한다.
	//SAFE_DELETE(m_SBufferMtrlScalar);
}



bool CMaterial::SaveJson(Json::Value* _pJson)
{
	if (nullptr == _pJson)
		return false;
	else if (false == CRes::SaveJson(_pJson))
		return false;

	if (nullptr == m_pShader)
		return false;

	Json::Value& jVal = *_pJson;

	jVal[string(RES_INFO::MATERIAL::JSON_KEY::strKey_Shader)] = m_pShader->GetKey();
	//jVal[string(RES_INFO::MATERIAL::JSON_KEY::bUseInstancing)] = m_bUseInstancing;
	jVal[string(RES_INFO::MATERIAL::JSON_KEY::arrStrKeyTex)] = Json::Value(Json::arrayValue);

	for (int i = 0; i < (int)eMTRLDATA_PARAM_TEX::_END; ++i)
	{
		if (nullptr != m_arrTex[i])
		{
			(*_pJson)[string(RES_INFO::MATERIAL::JSON_KEY::arrStrKeyTex)].append(m_arrTex[i]->GetKey());
		}
		else
		{
			(*_pJson)[string(RES_INFO::MATERIAL::JSON_KEY::arrStrKeyTex)].append("");
		}
	}

	return true;
}


bool CMaterial::LoadJson(Json::Value* _pJson)
{
	if (nullptr == _pJson)
		return false;
	else if (false == CRes::LoadJson(_pJson))
		return false;

	const Json::Value& jVal = *_pJson;

	CResMgr* pResMgr = CResMgr::GetInst();
	
	m_pShader = pResMgr->Load<CGraphicsShader>(jVal[string(RES_INFO::MATERIAL::JSON_KEY::strKey_Shader)].asString());

	//m_bUseInstancing = jVal[string(RES_INFO::MATERIAL::JSON_KEY::bUseInstancing)].asBool();
	

	for (int i = 0; i < (int)eMTRLDATA_PARAM_TEX::_END; ++i)
	{
		string strKeyTex = jVal[string(RES_INFO::MATERIAL::JSON_KEY::arrStrKeyTex)][i].asString();
		if (false == strKeyTex.empty())
		{
			Ptr<CTexture> pTex = pResMgr->FindRes<CTexture>(strKeyTex);
			if (nullptr == pTex)
			{
				SetTexParam((eMTRLDATA_PARAM_TEX)i, pResMgr->Load<CTexture>(strKeyTex));
			}
		}
	}
	return true;
}



void CMaterial::BindData()
{
	if (nullptr == m_pShader)
		return;
	
	//쉐이더도 데이터를 바인딩해준다.
	m_pShader->BindData();

	//텍스처정보도 바인딩 걸어준다.
	for (int i = 0; i < (int)eMTRLDATA_PARAM_TEX::_END; ++i)
	{
		if (true == IsTexture((eMTRLDATA_PARAM_TEX)i))
			m_arrTex[i]->BindData_SRV(i, eSHADER_PIPELINE_STAGE::__PIXEL);
	}
	//Texture Const Buffer Update
	CConstBuffer* CMtrlTexBuffer = CDevice::GetInst()->GetConstBuffer(idx_b_CBUFFER_MTRL_TEX);
	CMtrlTexBuffer->UploadData(&m_MtrlTex);
	CMtrlTexBuffer->BindBuffer();
}



//void CMaterial::SetInstancingMode(bool _bUse)
//{
//	m_bUseInstancing = _bUse;
//	if (m_bUseInstancing)
//	{
//		m_CBufferMtrlScalar = nullptr;
//	}
//	else
//	{
//		m_CBufferMtrlScalar = CDevice::GetInst()->GetConstBuffer(idx_b_CBUFFER_MTRL_SCALAR);
//	}
//}



void CMaterial::SetTexParam(eMTRLDATA_PARAM_TEX _Param, Ptr<CTexture> _Tex)
{
	m_arrTex[(int)_Param] = _Tex;

	//nullptr일 경우 해제
	BOOL bIsExist = (nullptr != _Tex);

	switch (_Param)
	{
	case eMTRLDATA_PARAM_TEX::_0: m_MtrlTex.bTEX_0 = bIsExist; break;
	case eMTRLDATA_PARAM_TEX::_1: m_MtrlTex.bTEX_1 = bIsExist; break;
	case eMTRLDATA_PARAM_TEX::_2: m_MtrlTex.bTEX_2 = bIsExist; break;
	case eMTRLDATA_PARAM_TEX::_3: m_MtrlTex.bTEX_3 = bIsExist; break;
	case eMTRLDATA_PARAM_TEX::_4: m_MtrlTex.bTEX_4 = bIsExist; break;
	case eMTRLDATA_PARAM_TEX::_5: m_MtrlTex.bTEX_5 = bIsExist; break;
	case eMTRLDATA_PARAM_TEX::_6: m_MtrlTex.bTEX_6 = bIsExist; break;
	case eMTRLDATA_PARAM_TEX::_7: m_MtrlTex.bTEX_7 = bIsExist; break;
	case eMTRLDATA_PARAM_TEX::_END: break;
	default: break;
	}
}
