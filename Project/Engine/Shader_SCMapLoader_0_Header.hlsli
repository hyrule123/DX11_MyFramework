#ifndef SHADER_SCMAPLOADER_HEADER
#define SHADER_SCMAPLOADER_HEADER

#include "Shader_Header_Struct.hlsli"

#ifdef __cplusplus

#else

#endif

//단위크기 : 20 byte(dummy) + 2 byte * 16 = 52 byte
//총 갯수 : 4096개
struct CV5
{
	//Dummy1 + 2 = 20byte
	UINT32_4 Dummy1;
	UINT32 Dummy2;
	
	//Megatile
	UINT16_16 MegaTileIndex;

};

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
struct WPE
{
	UINT32 RGBA;
};


#endif