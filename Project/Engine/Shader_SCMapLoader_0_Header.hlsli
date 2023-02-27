#ifndef SHADER_SCMAPLOADER_HEADER
#define SHADER_SCMAPLOADER_HEADER

#include "Shader_Header_Struct.hlsli"

#ifdef __cplusplus

#else

#endif

//����ũ�� : 20 byte(dummy) + 2 byte * 16 = 52 byte
//�� ���� : 4096��
struct CV5
{
	//Dummy1 + 2 = 20byte
	UINT32_4 Dummy1;
	UINT32 Dummy2;
	
	//Megatile
	UINT16_16 MegaTileIndex;

};

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
struct WPE
{
	UINT32 RGBA;
};


#endif