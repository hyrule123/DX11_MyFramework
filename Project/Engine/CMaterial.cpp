#include "pch.h"
#include "CMaterial.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CMaterial::CMaterial()
	: CRes(eRES_TYPE::MATERIAL)
	, m_Const{}
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
		if (1 == m_Const.arrbTex[i])
			m_arrTex[i]->BindData_SRV(i, eSHADER_PIPELINE_STAGE::__PIXEL);
	}

	// Constant Update
	CConstBuffer* pMtrlBuffer = CDevice::GetInst()->GetConstBuffer(eCONST_BUFFER_TYPE::MATERIAL);
	pMtrlBuffer->UploadData(&m_Const);
	pMtrlBuffer->BindBuffer();
}


void CMaterial::SetScalarParam(eMTRLDATA_PARAM_SCALAR _Param, const void* _Src)
{
	switch (_Param)
	{
	case eMTRLDATA_PARAM_SCALAR::INT_0:
	case eMTRLDATA_PARAM_SCALAR::INT_1:
	case eMTRLDATA_PARAM_SCALAR::INT_2:
	case eMTRLDATA_PARAM_SCALAR::INT_3:
		m_Const.arrInt[(int)_Param] = *((int*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::FLOAT_0:
	case eMTRLDATA_PARAM_SCALAR::FLOAT_1:
	case eMTRLDATA_PARAM_SCALAR::FLOAT_2:
	case eMTRLDATA_PARAM_SCALAR::FLOAT_3:
		m_Const.arrFloat[(int)_Param - (int)eMTRLDATA_PARAM_SCALAR::FLOAT_0] = *((float*)_Src);
		break;

	case eMTRLDATA_PARAM_SCALAR::VEC2_0:
	case eMTRLDATA_PARAM_SCALAR::VEC2_1:
	case eMTRLDATA_PARAM_SCALAR::VEC2_2:
	case eMTRLDATA_PARAM_SCALAR::VEC2_3:
		m_Const.arrV2[(int)_Param - (int)eMTRLDATA_PARAM_SCALAR::VEC2_0] = *((Vec2*)_Src);
		break;

	case eMTRLDATA_PARAM_SCALAR::VEC4_0:
	case eMTRLDATA_PARAM_SCALAR::VEC4_1:
	case eMTRLDATA_PARAM_SCALAR::VEC4_2:
	case eMTRLDATA_PARAM_SCALAR::VEC4_3:
		m_Const.arrV4[(int)_Param - (int)eMTRLDATA_PARAM_SCALAR::VEC4_0] = *((Vec4*)_Src);
		break;

	case eMTRLDATA_PARAM_SCALAR::MAT_0:
	case eMTRLDATA_PARAM_SCALAR::MAT_1:
	case eMTRLDATA_PARAM_SCALAR::MAT_2:
	case eMTRLDATA_PARAM_SCALAR::MAT_3:
		m_Const.arrMat[(int)_Param - (int)eMTRLDATA_PARAM_SCALAR::MAT_0] = *((Matrix*)_Src);
		break;	


	}
}


void CMaterial::SetTexParam(eMTRLDATA_PARAM_TEX _Param, Ptr<CTexture> _Tex)
{
	assert(nullptr != _Tex);

	m_arrTex[(int)_Param] = _Tex;
	m_Const.arrbTex[(int)_Param] = 1;
}