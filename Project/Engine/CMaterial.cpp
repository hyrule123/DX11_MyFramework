#include "pch.h"
#include "CMaterial.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CMaterial::CMaterial()
	: CRes(eRES_TYPE::MATERIAL)
	, m_MtrlScalar{}
	, m_arrTex{}
{	

}



CMaterial::~CMaterial()
{
}


void CMaterial::BindData()
{
	if (nullptr == m_pShader)
		return;

	m_pShader->BindData();

	// Texture Update
	for (int i = 0; i < (int)eMTRLDATA_PARAM_TEX::_END; ++i)
	{
		if (true == IsTexture((eMTRLDATA_PARAM_TEX)i))
			m_arrTex[i]->BindData_SRV(i, eSHADER_PIPELINE_STAGE::__PIXEL);
	}
	//Texture Const Buffer Update
	CConstBuffer* pMtrlTexBuffer = CDevice::GetInst()->GetConstBuffer(e_b_CBUFFER_MTRL_TEX);
	pMtrlTexBuffer->UploadData(&m_MtrlTex);
	pMtrlTexBuffer->BindBuffer();

	// Constant Update
	CConstBuffer* pMtrlScalarBuffer = CDevice::GetInst()->GetConstBuffer(e_b_CBUFFER_MTRL_SCALAR);
	pMtrlScalarBuffer->UploadData(&m_MtrlScalar);
	pMtrlScalarBuffer->BindBuffer();
}




void CMaterial::SetScalarParam(eMTRLDATA_PARAM_SCALAR _Param, const void* _Src)
{
	switch (_Param)
	{
	case eMTRLDATA_PARAM_SCALAR::INT_0: m_MtrlScalar.INT_0 = *((int*)_Src); 
		break;
	case eMTRLDATA_PARAM_SCALAR::INT_1: m_MtrlScalar.INT_1 = *((int*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::INT_2: m_MtrlScalar.INT_2 = *((int*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::INT_3: m_MtrlScalar.INT_3 = *((int*)_Src);
		break;


	case eMTRLDATA_PARAM_SCALAR::FLOAT_0: m_MtrlScalar.FLOAT_0 = *((float*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::FLOAT_1: m_MtrlScalar.FLOAT_1 = *((float*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::FLOAT_2: m_MtrlScalar.FLOAT_2 = *((float*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::FLOAT_3: m_MtrlScalar.FLOAT_3 = *((float*)_Src);
		break;


	case eMTRLDATA_PARAM_SCALAR::VEC2_0: m_MtrlScalar.VEC2_0 = *((Vec2*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC2_1: m_MtrlScalar.VEC2_1 = *((Vec2*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC2_2: m_MtrlScalar.VEC2_2 = *((Vec2*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC2_3: m_MtrlScalar.VEC2_3 = *((Vec2*)_Src);
		break;

	case eMTRLDATA_PARAM_SCALAR::VEC4_0: m_MtrlScalar.VEC4_0 = *((Vec4*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC4_1: m_MtrlScalar.VEC4_1 = *((Vec4*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC4_2: m_MtrlScalar.VEC4_2 = *((Vec4*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC4_3: m_MtrlScalar.VEC4_3 = *((Vec4*)_Src);
		break;

	case eMTRLDATA_PARAM_SCALAR::MAT_0: m_MtrlScalar.MAT_0 = *((Matrix*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::MAT_1: m_MtrlScalar.MAT_1 = *((Matrix*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::MAT_2: m_MtrlScalar.MAT_2 = *((Matrix*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::MAT_3: m_MtrlScalar.MAT_3 = *((Matrix*)_Src);
		break;	

	}
}


void CMaterial::SetTexParam(eMTRLDATA_PARAM_TEX _Param, Ptr<CTexture> _Tex)
{
	assert(nullptr != _Tex);

	m_arrTex[(int)_Param] = _Tex;

	switch (_Param)
	{
	case eMTRLDATA_PARAM_TEX::_0: m_MtrlTex.bTEX_0 = TRUE; break;
	case eMTRLDATA_PARAM_TEX::_1: m_MtrlTex.bTEX_1 = TRUE; break;
	case eMTRLDATA_PARAM_TEX::_2: m_MtrlTex.bTEX_2 = TRUE; break;
	case eMTRLDATA_PARAM_TEX::_3: m_MtrlTex.bTEX_3 = TRUE; break;
	case eMTRLDATA_PARAM_TEX::_4: m_MtrlTex.bTEX_4 = TRUE; break;
	case eMTRLDATA_PARAM_TEX::_5: m_MtrlTex.bTEX_5 = TRUE; break;
	case eMTRLDATA_PARAM_TEX::_6: m_MtrlTex.bTEX_6 = TRUE; break;
	case eMTRLDATA_PARAM_TEX::_7: m_MtrlTex.bTEX_7 = TRUE; break;
	case eMTRLDATA_PARAM_TEX::_END: break;
	default: break;
	}
}

