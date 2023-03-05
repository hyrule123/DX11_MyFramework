#include "pch.h"
#include "CCS_Initialize.h"

#include "CStructBuffer.h"

#include "Shader_Header_Struct.hlsli"



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
	////INT64�� 1�� �־ �����Ѵ�. HLSL������ �� ���� INT32���·� �о���δ�.
	g_InitSetting.bIsLittleEndian = static_cast<UINT64>(1u);


	m_pSBuffer_InitSetting = new CStructBuffer(tSBufferDesc{ eSTRUCT_BUFFER_TYPE::READ_WRITE, eSHADER_PIPELINE_STAGE::__ALL, eCBUFFER_SBUFFER_SHAREDATA_IDX::NONE, e_t_INIT_SETTING, e_u_INIT_SETTING });
	m_pSBuffer_InitSetting->Create(sizeof(tInitSetting), 1u, &g_InitSetting, 1u);

	//UAV�� ���ε�
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

	//	//C++�� int �迭�� 4 byte ���� - HLSL �������� �����ֱ� ����
	//	UINT32_4 NotInUse0[4];
	//	UINT32_4 NotInUse1[4];

	//	UINT32_4 NotInUse2[2];
	//	UINT32_4 NotInUse3[2];
	//};

	//tInitSettingsEX tSet = {};
	//m_pSBuffer_InitSetting->GetData(&tSet, sizeof(tSet));

	//int a = 0;


	//UNBIND�ϰ� SRV�� ���ε� �ɾ��ش�.
	m_pSBuffer_InitSetting->UnBindUAV();
	m_pSBuffer_InitSetting->BindBufferSRV();

	//C++������ endianness �׽�Ʈ
	//��� �ַ� ����ϴ� Intel, Amd CPU�� ��� little endian ����̶� ������ �ʿ� ����.
	unsigned char IsLittleEndian = *(unsigned char*)&(g_InitSetting.bIsLittleEndian);
	if (static_cast<unsigned char>(1u) == IsLittleEndian)
		g_InitSetting.bIsLittleEndian = static_cast<UINT64>(1u);
}
