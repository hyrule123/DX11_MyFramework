#ifndef SHADER_SCMAPLOADER_HEADER
#define SHADER_SCMAPLOADER_HEADER

#include "Shader_Header_Register.hlsli"

//��ǻƮ���̴����� 0 ~ 2 3������ �����
#define MTRL_SCALAR_INT_MAPSIZE_PACK MTRLDATA_PARAM_SCALAR(INT_3)

//����ũ�� : 2byte
//�� ���� : ���� ����ũ��(�ް�Ÿ��) * ����ũ��(�ް�Ÿ��)
//HLSL������ 8���� ��ŷ�ؼ� ����(4����Ʈ int 4��)
#define MXTM_PACK 8u
//MXTM_MAX�� �Ź� �޶����Ƿ� ����(�ʻ�����)
struct MXTM
{
	UINT32_4 MXTM_Pack;
};

//����ũ�� : 20 byte(dummy) + 2 byte * 16 = 52 byte
//�� ���� : 4096��
//HLSL������ ���� �����ʹ� ������ �ް�Ÿ�� ������ ����ִ� ������ ������ �����ؼ� �鿩�� ����
//2byte * 16 = 32 byte
#define CV5_MAX 4096u
struct CV5
{
	//Megatile
	UINT32_8 MegaTileIndex;
};


//����ũ��: 2 byte(��Ʈ �÷���) * 16(�̴�Ÿ���� ����) = 32byte, ������ɿ���, ����
//�� ���� : 65536��
//HLSL������ 4byte * 8���� ��ŷ�ؼ� ������ ���� -> ���� = 65536 / 16 = 4096
#define VF4_PACK 16u
#define VF4_MAX 4096u
struct VF4
{
	//32 byte length
	UINT32_8 MiniTileFlags;
};


//����ũ��: 2 byte * 16 = 32byte, megatile�� � minitile�� �̷�������� ǥ��
//�� ���� : 65536��
#define VX4_MAX 65536u
struct VX4
{
	UINT32_8 VR4Index;
};

//���� ũ��: 1 byte * 64 = 64byte - mini tile������ �ȼ��� WPE �ȷ�Ʈ�� �� �� ���� �ش��ϴ����� ����(0 ~ 255 ������ �ε����� ����Ǿ�����)
//�̴�Ÿ���� 8 * 8 �ȼ���. �׷��ϱ� �ϳ��� �̴�Ÿ�Ͽ��� �� Ÿ���� WPE���� �����;� �ϴ� ������ �ε����� 0 ~ 255 ���̷� �����ϴ� ��.
//32768��
#define VR4_MAX 32768u
#define VR4_PACK 4u
struct VR4
{
	UINT32_16 ColorPack;
};



//WPE Data : ����ũ�� 1 byte * 4 = 4 byte (RGBA) - A�� ����(�ܼ� �е�).
//256��
//4Byte�� �������� �����Ƿ� 4�� ������ �ϳ��� ��� ������� �Ѵ�. -> 256 / 4 = 64
#define WPE_PACK 4u
#define WPE_MAX 64u
struct WPE
{
	UINT32_4 RGBAPack;
};

#define e_t_SBUFFER_MXTM REGISTER_IDX(t, 0)
#define e_t_SBUFFER_CV5 REGISTER_IDX(t, 1)
#define e_t_SBUFFER_VX4 REGISTER_IDX(t, 2)
#define e_t_SBUFFER_VR4 REGISTER_IDX(t, 3)
#define e_t_SBUFFER_WPE REGISTER_IDX(t, 4)
#define e_t_SBUFFER_VF4 REGISTER_IDX(t, 5)

#define e_t_TEXTURE_TARGET REGISTER_IDX(t, 6)
#define e_u_TEXTURERW_TARGET REGISTER_IDX(u, 0)

#define e_u_SBUFFERRW_DEBUG REGISTER_IDX(u, 1)


#ifdef __cplusplus



#else

StructuredBuffer<MXTM> g_SBuffer_MXTM : register(e_t_SBUFFER_MXTM);
StructuredBuffer<CV5> g_SBuffer_CV5 : register(e_t_SBUFFER_CV5);
StructuredBuffer<VX4> g_SBuffer_VX4 : register(e_t_SBUFFER_VX4);
StructuredBuffer<VR4> g_SBuffer_VR4 : register(e_t_SBUFFER_VR4);
StructuredBuffer<WPE> g_SBuffer_WPE : register(e_t_SBUFFER_WPE);
StructuredBuffer<VX4> g_SBuffer_VF4 : register(e_t_SBUFFER_VF4);

RWStructuredBuffer<tMtrlScalarData> g_SBufferRW_Debug : register(e_u_SBUFFERRW_DEBUG);
RWTexture2D<float4> g_TexRW_SCMap : register(e_u_TEXTURERW_TARGET);
	
#endif


#endif