#include "pch.h"
#include "CMaterial.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CMaterial::CMaterial()
	: CRes(eRES_TYPE::MATERIAL)
	, m_MtrlData{}
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

	// Constant Update
	CConstBuffer* pMtrlBuffer = CDevice::GetInst()->GetConstBuffer(e_b_CBUFFER_MATERIAL);
	pMtrlBuffer->UploadData(&m_MtrlData);
	pMtrlBuffer->BindBuffer();
}




void CMaterial::SetScalarParam(eMTRLDATA_PARAM_SCALAR _Param, const void* _Src)
{
	switch (_Param)
	{
	case eMTRLDATA_PARAM_SCALAR::INT_0: m_MtrlData.INT_0 = *((int*)_Src); 
		break;
	case eMTRLDATA_PARAM_SCALAR::INT_1: m_MtrlData.INT_1 = *((int*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::INT_2: m_MtrlData.INT_2 = *((int*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::INT_3: m_MtrlData.INT_3 = *((int*)_Src);
		break;


	case eMTRLDATA_PARAM_SCALAR::FLOAT_0: m_MtrlData.FLOAT_0 = *((float*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::FLOAT_1: m_MtrlData.FLOAT_1 = *((float*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::FLOAT_2: m_MtrlData.FLOAT_2 = *((float*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::FLOAT_3: m_MtrlData.FLOAT_3 = *((float*)_Src);
		break;


	case eMTRLDATA_PARAM_SCALAR::VEC2_0: m_MtrlData.VEC2_0 = *((Vec2*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC2_1: m_MtrlData.VEC2_1 = *((Vec2*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC2_2: m_MtrlData.VEC2_2 = *((Vec2*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC2_3: m_MtrlData.VEC2_3 = *((Vec2*)_Src);
		break;

	case eMTRLDATA_PARAM_SCALAR::VEC4_0: m_MtrlData.VEC4_0 = *((Vec4*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC4_1: m_MtrlData.VEC4_1 = *((Vec4*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC4_2: m_MtrlData.VEC4_2 = *((Vec4*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC4_3: m_MtrlData.VEC4_3 = *((Vec4*)_Src);
		break;

	case eMTRLDATA_PARAM_SCALAR::MAT_0: m_MtrlData.MAT_0 = *((Matrix*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::MAT_1: m_MtrlData.MAT_1 = *((Matrix*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::MAT_2: m_MtrlData.MAT_2 = *((Matrix*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::MAT_3: m_MtrlData.MAT_3 = *((Matrix*)_Src);
		break;	

	}
}


void CMaterial::SetTexParam(eMTRLDATA_PARAM_TEX _Param, Ptr<CTexture> _Tex)
{
	assert(nullptr != _Tex);

	m_arrTex[(int)_Param] = _Tex;

	switch (_Param)
	{
	case eMTRLDATA_PARAM_TEX::_0: m_MtrlData.bTEX_0 = 1; break;
	case eMTRLDATA_PARAM_TEX::_1: m_MtrlData.bTEX_1 = 1; break;
	case eMTRLDATA_PARAM_TEX::_2: m_MtrlData.bTEX_2 = 1; break;
	case eMTRLDATA_PARAM_TEX::_3: m_MtrlData.bTEX_3 = 1; break;
	case eMTRLDATA_PARAM_TEX::_4: m_MtrlData.bTEX_4 = 1; break;
	case eMTRLDATA_PARAM_TEX::_5: m_MtrlData.bTEX_5 = 1; break;
	case eMTRLDATA_PARAM_TEX::_6: m_MtrlData.bTEX_6 = 1; break;
	case eMTRLDATA_PARAM_TEX::_7: m_MtrlData.bTEX_7 = 1; break;
	case eMTRLDATA_PARAM_TEX::_END: break;
	default: break;
	}
}