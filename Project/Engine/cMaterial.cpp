#include "pch.h"
#include "cMaterial.h"

#include "cDevice.h"
#include "cConstBuffer.h"
#include "cStructBuffer.h"

#include "jsoncpp.h"

#include "cResMgr.h"


cMaterial::cMaterial()
	: IRes(eRES_TYPE::MATERIAL)
	, m_MtrlTex{}
	, m_arrTex{}
	, m_bIsDynamic()
{
}

cMaterial::cMaterial(const cMaterial& _Clone)
	: IRes(_Clone)
	, m_pShader(_Clone.m_pShader)
	, m_MtrlTex(_Clone.m_MtrlTex)
	, m_bIsDynamic(true)
{
	for (int i = 0; i < (int)eMTRLDATA_PARAM_TEX::_END; ++i)
	{
		m_arrTex[i] = _Clone.m_arrTex[i];
	}
}

bool cMaterial::Save(const std::filesystem::path& _fileName)
{
	std::filesystem::path ExtAdd = _fileName;
	ExtAdd.replace_extension(RES_INFO::MATERIAL::Ext);
	return IRes::Save(ExtAdd);
}

bool cMaterial::Load(const std::filesystem::path& _fileName)
{
	std::filesystem::path ExtAdd = _fileName;
	ExtAdd.replace_extension(RES_INFO::MATERIAL::Ext);
	return IRes::Load(ExtAdd);
}




cMaterial::~cMaterial()
{
	//상수버퍼는 cDevice에서 관리하고, 구조화버퍼는 여기서 관리하므로 구조화버퍼만 제거한다.
	//SAFE_DELETE(m_SBufferMtrlScalar);
}



bool cMaterial::SaveJson(Json::Value* _pJson)
{
	if (nullptr == _pJson)
		return false;
	else if (false == IRes::SaveJson(_pJson))
		return false;

	if (nullptr == m_pShader)
		return false;

	Json::Value& jVal = *_pJson;

	jVal[JsonKey_Material::strKey_Shader] = string(m_pShader->GetKey());
	jVal[JsonKey_Material::arrStrKeyTex] = Json::Value(Json::arrayValue);

	for (int i = 0; i < (int)eMTRLDATA_PARAM_TEX::_END; ++i)
	{
		if (nullptr != m_arrTex[i])
		{
			(*_pJson)[JsonKey_Material::arrStrKeyTex].append(string(m_arrTex[i]->GetKey()));
		}
		else
		{
			(*_pJson)[JsonKey_Material::arrStrKeyTex].append("");
		}
	}

	return true;
}


bool cMaterial::LoadJson(Json::Value* _pJson)
{
	if (nullptr == _pJson)
		return false;
	else if (false == IRes::LoadJson(_pJson))
		return false;

	const Json::Value& jVal = *_pJson;

	cResMgr* pResMgr = cResMgr::GetInst();
	
	m_pShader = pResMgr->Load<cGraphicsShader>(jVal[JsonKey_Material::strKey_Shader].asString());


	for (int i = 0; i < (int)eMTRLDATA_PARAM_TEX::_END; ++i)
	{
		string strKeyTex = jVal[JsonKey_Material::arrStrKeyTex][i].asString();
		if (false == strKeyTex.empty())
		{
			Ptr<cTexture> pTex = pResMgr->Load<cTexture>(strKeyTex);
			assert(nullptr != pTex);

			SetTexParam((eMTRLDATA_PARAM_TEX)i, pTex);

		}
	}
	return true;
}



void cMaterial::BindData()
{
	if (nullptr == m_pShader)
		return;
	
	//쉐이더도 데이터를 바인딩해준다.
	m_pShader->BindData();

	//텍스처정보도 바인딩 걸어준다.
	for (int i = 0; i < (int)eMTRLDATA_PARAM_TEX::_END; ++i)
	{
		if (true == IsTexture((eMTRLDATA_PARAM_TEX)i))
			m_arrTex[i]->BindData_SRV(i, define_Shader::ePIPELINE_STAGE_FLAG::__PIXEL);
	}
	//cTexture Const Buffer Update
	cConstBuffer* CMtrlTexBuffer = cDevice::GetInst()->GetConstBuffer(REGISLOT_b_CBUFFER_MTRL_TEX);
	CMtrlTexBuffer->UploadData(&m_MtrlTex);
	CMtrlTexBuffer->BindBuffer();
}



//void cMaterial::SetInstancingMode(bool _bUse)
//{
//	m_bUseInstancing = _bUse;
//	if (m_bUseInstancing)
//	{
//		m_CBufferMtrlScalar = nullptr;
//	}
//	else
//	{
//		m_CBufferMtrlScalar = cDevice::GetInst()->GetConstBuffer(REGISLOT_b_CBUFFER_MTRL_SCALAR);
//	}
//}



void cMaterial::SetTexParam(eMTRLDATA_PARAM_TEX _Param, Ptr<cTexture> _Tex)
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
