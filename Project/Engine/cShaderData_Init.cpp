#include "pch.h"
#include "cShaderData_Init.h"

#include "cStructBuffer.h"

#include "S_H_Struct.hlsli"

#include "strKey_Default.h"

#ifdef _DEBUG
#include "DefaultShader/S_C_Initalize_Debug.h"
#else
#include "DefaultShader/S_C_Initalize_Release.h"
#endif
#include "S_H_Initialize.hlsli"

#include "cComputeShader.h"

cShaderData_Init::cShaderData_Init()
	: m_pSBuffer_InitSetting()
{
	m_pSBuffer_InitSetting = std::make_unique<cStructBuffer>();

	////INT64에 1을 넣어서 전달한다. HLSL에서는 이 값을 INT32형태로 읽어들인다.
	g_InitSetting.bIsLittleEndian = static_cast<UINT64>(1u);

	tSBufferDesc Desc = {};

	Desc.eSBufferType = eSTRUCT_BUFFER_TYPE::READ_WRITE;
	Desc.flag_PipelineBindTarget_SRV = define_Shader::ePIPELINE_STAGE_FLAG::__ALL;
	Desc.REGISLOT_t_SRV = REGISLOT_t_INIT_SETTING;
	Desc.REGISLOT_u_UAV = REGISLOT_u_INIT_SETTING;

	m_pSBuffer_InitSetting->SetDesc(Desc);

	if FAILED(m_pSBuffer_InitSetting->Create(sizeof(tInitSetting), 1u, &g_InitSetting, 1u))
	{
		ERROR_MESSAGE_A("HLSL Initialization Failed.");
	}
}

cShaderData_Init::~cShaderData_Init()
{

}


tNumDataCS cShaderData_Init::BindDataCS()
{
	//UAV와 바인딩
	m_pSBuffer_InitSetting->BindBufferUAV();

	return tNumDataCS(1u, 1u, 1u);
}

bool cShaderData_Init::BindDataGS()
{
	m_pSBuffer_InitSetting->BindBufferSRV();
	return true;
}

void cShaderData_Init::UnBind()
{
	//UNBIND하고 SRV와 바인딩 걸어준다.
	m_pSBuffer_InitSetting->UnBind();

	//C++에서도 endianness 테스트
	//사실 주로 사용하는 Intel, Amd CPU는 모두 little endian 방식이라 걱정할 필요 없음.
	unsigned char IsLittleEndian = *(unsigned char*)&(g_InitSetting.bIsLittleEndian);
	if (static_cast<unsigned char>(1u) == IsLittleEndian)
		g_InitSetting.bIsLittleEndian = static_cast<UINT64>(1u);
}
