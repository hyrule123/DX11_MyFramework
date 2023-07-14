#include "pch.h"
#include "cCSModule_Initialize.h"

#include "cStructBuffer.h"

#include "S_H_Struct.hlsli"

#include "strKey_Default.h"

#ifdef _DEBUG
#include "DefaultShader/S_C_Initalize_Debug.h"
#else
#include "DefaultShader/S_C_Initalize_Release.h"
#endif

#include "cComputeShader.h"



cCSModule_Initialize::cCSModule_Initialize()
	: m_pSBuffer_InitSetting()
{
}

cCSModule_Initialize::~cCSModule_Initialize()
{
}


bool cCSModule_Initialize::Init()
{
	GetOwner()->SetEngineDefaultRes(true);
	GetOwner()->CreateShaderFromHeader(g_CS_HLSL_Init, sizeof(g_CS_HLSL_Init));

	return false;
}

bool cCSModule_Initialize::BindDataCS()
{
	////INT64에 1을 넣어서 전달한다. HLSL에서는 이 값을 INT32형태로 읽어들인다.
	g_InitSetting.bIsLittleEndian = static_cast<UINT64>(1u);

	tSBufferDesc Desc = {};

	Desc.eSBufferType = eSTRUCT_BUFFER_TYPE::READ_WRITE;
	Desc.flag_PipelineBindTarget_SRV = define_Shader::ePIPELINE_STAGE_FLAG::__ALL;
	Desc.REGISLOT_t_SRV = REGISLOT_t_INIT_SETTING;
	Desc.REGISLOT_u_UAV = REGISLOT_u_INIT_SETTING;

	m_pSBuffer_InitSetting = std::make_unique<cStructBuffer>();
	m_pSBuffer_InitSetting->SetDesc(Desc);
	m_pSBuffer_InitSetting->Create(sizeof(tInitSetting), 1u, &g_InitSetting, 1u);

	//UAV와 바인딩
	m_pSBuffer_InitSetting->BindBufferUAV();

	GetOwner()->CalcGroupNumber(1u, 1u, 1u);

	return true;
}

void cCSModule_Initialize::UnBind()
{
	//UNBIND하고 SRV와 바인딩 걸어준다.
	m_pSBuffer_InitSetting->UnBind();
	m_pSBuffer_InitSetting->BindBufferSRV();

	//C++에서도 endianness 테스트
	//사실 주로 사용하는 Intel, Amd CPU는 모두 little endian 방식이라 걱정할 필요 없음.
	unsigned char IsLittleEndian = *(unsigned char*)&(g_InitSetting.bIsLittleEndian);
	if (static_cast<unsigned char>(1u) == IsLittleEndian)
		g_InitSetting.bIsLittleEndian = static_cast<UINT64>(1u);
}
