#ifndef SHADER_SCMAPLOADER_HEADER
#define SHADER_SCMAPLOADER_HEADER

#include "Shader_Header_Register.hlsli"

struct CV5
{
	//Megatile
	UINT16_16 MegaTileIndex;
};
#define e_t_SBUFFER_CV5 REGISTER_IDX(t, 0)
#define e_t_SBUFFER_VX4 REGISTER_IDX(t, 1)
#define e_t_SBUFFER_VR4 REGISTER_IDX(t, 2)
#define e_t_SBUFFER_WPE REGISTER_IDX(t, 3)
#define e_t_SBUFFER_VF4 REGISTER_IDX(t, 4)
#define e_t_TEXTURE_TARGET REGISTER_IDX(t, 5)


#ifdef __cplusplus



#else

StructuredBuffer<CV5> g_SBuffer_CV5 : register(e_t_SBUFFER_CV5);
	
#endif




//����ũ�� : 20 byte(dummy) + 2 byte * 16 = 52 byte
//�� ���� : 4096��



//����ũ��: 2 byte * 16 = 32byte, ������ɿ���, ����
//�� ���� : 65536��
struct VF4
{
	//32 byte length
	UINT16_16 MiniTileFlags;
};

//����ũ��: 2 byte * 16 = 32byte, megatile�� � minitile�� �̷�������� ǥ��
//�� ���� : 65536��
struct VX4
{
	UINT16_16 VR4Index;
};

//���� ũ��: 1 byte * 64 = 64byte - mini tile������ �ȼ��� WPE �ȷ�Ʈ�� �� �� ���� �ش��ϴ����� ����(0 ~ 255 ������ �ε����� ����Ǿ�����)
//32768��
struct VR4
{
	UINT32_16 Color;
};



//WPE Data : ����ũ�� 1 byte * 4 = 4 byte (RGBA) - A�� ����.
//256��
//4Byte�� �������� �����Ƿ� 4�� ������ �ϳ��� ��� ������� �Ѵ�. -> 256 / 4 = 64

struct WPE
{
	UINT32_4 RGBA;
};


#endif