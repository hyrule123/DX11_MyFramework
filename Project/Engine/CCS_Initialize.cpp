#include "pch.h"
#include "CCS_Initialize.h"

#include "CStructBuffer.h"

#include "S_H_Struct.hlsli"



CCS_Initialize::CCS_Initialize()
	: CComputeShader(1u, 1u, 1u)
	, m_pSBuffer_InitSetting()
{
}

CCS_Initialize::~CCS_Initialize()
{
	DESTRUCTOR_DELETE(m_pSBuffer_InitSetting);
}

bool CCS_Initialize::BindDataCS()
{
	////INT64에 1을 넣어서 전달한다. HLSL에서는 이 값을 INT32형태로 읽어들인다.
	g_InitSetting.bIsLittleEndian = static_cast<UINT64>(1u);


	m_pSBuffer_InitSetting = new CStructBuffer(tSBufferDesc{ eSTRUCT_BUFFER_TYPE::READ_WRITE, eSHADER_PIPELINE_STAGE::__ALL, eCBUFFER_SBUFFER_SHAREDATA_IDX::NONE, e_t_INIT_SETTING, e_u_INIT_SETTING });
	m_pSBuffer_InitSetting->Create(sizeof(tInitSetting), 1u, &g_InitSetting, 1u);

	//UAV와 바인딩
	m_pSBuffer_InitSetting->BindBufferUAV();

	CalcGroupNumber(1u, 1u, 1u);

	return true;
}

void CCS_Initialize::UnBindCS()
{
	//struct tInitSettingsEX
	//{
	//	UINT32 bIsLittleEndian;
	//	float3 Padding0;

	//	//C++은 int 배열이 4 byte 단위 - HLSL 기준으로 맞춰주기 위함
	//	UINT32_4 NotInUse0[4];
	//	UINT32_4 NotInUse1[4];

	//	UINT32_4 NotInUse2[2];
	//	UINT32_4 NotInUse3[2];
	//};

	//tInitSettingsEX tSet = {};
	//m_pSBuffer_InitSetting->GetData(&tSet, sizeof(tSet));

	//int a = 0;


	//UNBIND하고 SRV와 바인딩 걸어준다.
	m_pSBuffer_InitSetting->UnBindUAV();
	m_pSBuffer_InitSetting->BindBufferSRV();

	//C++에서도 endianness 테스트
	//사실 주로 사용하는 Intel, Amd CPU는 모두 little endian 방식이라 걱정할 필요 없음.
	unsigned char IsLittleEndian = *(unsigned char*)&(g_InitSetting.bIsLittleEndian);
	if (static_cast<unsigned char>(1u) == IsLittleEndian)
		g_InitSetting.bIsLittleEndian = static_cast<UINT64>(1u);
}
