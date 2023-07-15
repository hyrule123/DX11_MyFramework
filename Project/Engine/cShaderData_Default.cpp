#include "pch.h"
#include "cShaderData_Default.h"

#include "cDevice.h"
#include "cConstBuffer.h"

cShaderData_Default::cShaderData_Default()
{
}

cShaderData_Default::~cShaderData_Default()
{
}

tNumData cShaderData_Default::BindDataCS()
{
	static cConstBuffer* pCBuffer = cDevice::GetInst()->GetConstBuffer(REGISLOT_b_CBUFFER_MTRL_SCALAR);
	assert(pCBuffer);



    return tNumData();
}

bool cShaderData_Default::BindDataGS()
{
    return false;
}

void cShaderData_Default::UnBind()
{
}

void cShaderData_Default::SetMtrlScalarParam(eMTRLDATA_PARAM_SCALAR _Param, const void* _Src)
{
	switch (_Param)
	{
	case eMTRLDATA_PARAM_SCALAR::INT_0: m_CBuffer_CSShared.INT_0 = *((int*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::INT_1: m_CBuffer_CSShared.INT_1 = *((int*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::INT_2: m_CBuffer_CSShared.INT_2 = *((int*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::INT_3: m_CBuffer_CSShared.INT_3 = *((int*)_Src);
		break;


	case eMTRLDATA_PARAM_SCALAR::FLOAT_0: m_CBuffer_CSShared.FLOAT_0 = *((float*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::FLOAT_1: m_CBuffer_CSShared.FLOAT_1 = *((float*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::FLOAT_2: m_CBuffer_CSShared.FLOAT_2 = *((float*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::FLOAT_3: m_CBuffer_CSShared.FLOAT_3 = *((float*)_Src);
		break;


	case eMTRLDATA_PARAM_SCALAR::VEC2_0: m_CBuffer_CSShared.VEC2_0 = *((Vec2*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC2_1: m_CBuffer_CSShared.VEC2_1 = *((Vec2*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC2_2: m_CBuffer_CSShared.VEC2_2 = *((Vec2*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC2_3: m_CBuffer_CSShared.VEC2_3 = *((Vec2*)_Src);
		break;

	case eMTRLDATA_PARAM_SCALAR::VEC4_0: m_CBuffer_CSShared.VEC4_0 = *((Vec4*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC4_1: m_CBuffer_CSShared.VEC4_1 = *((Vec4*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC4_2: m_CBuffer_CSShared.VEC4_2 = *((Vec4*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC4_3: m_CBuffer_CSShared.VEC4_3 = *((Vec4*)_Src);
		break;

	case eMTRLDATA_PARAM_SCALAR::MAT_0: m_CBuffer_CSShared.MAT_0 = *((Matrix*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::MAT_1: m_CBuffer_CSShared.MAT_1 = *((Matrix*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::MAT_2: m_CBuffer_CSShared.MAT_2 = *((Matrix*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::MAT_3: m_CBuffer_CSShared.MAT_3 = *((Matrix*)_Src);
		break;
	}
}
