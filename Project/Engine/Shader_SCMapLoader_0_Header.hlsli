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




//단위크기 : 20 byte(dummy) + 2 byte * 16 = 52 byte
//총 갯수 : 4096개



//단위크기: 2 byte * 16 = 32byte, 통과가능여부, 높이
//총 갯수 : 65536개
struct VF4
{
	//32 byte length
	UINT16_16 MiniTileFlags;
};

//단위크기: 2 byte * 16 = 32byte, megatile이 어떤 minitile로 이루어졌는지 표시
//총 갯수 : 65536개
struct VX4
{
	UINT16_16 VR4Index;
};

//단위 크기: 1 byte * 64 = 64byte - mini tile각각의 픽셀이 WPE 팔레트의 몇 번 색에 해당하는지를 저장(0 ~ 255 사이의 인덱스가 저장되어있음)
//32768개
struct VR4
{
	UINT32_16 Color;
};



//WPE Data : 단위크기 1 byte * 4 = 4 byte (RGBA) - A는 없음.
//256개
//4Byte만 보낼수는 없으므로 4개 묶음을 하나로 묶어서 보내줘야 한다. -> 256 / 4 = 64

struct WPE
{
	UINT32_4 RGBA;
};


#endif