#ifndef S_0_H_SCMAPLOADER
#define S_0_H_SCMAPLOADER

#include "S_H_Register.hlsli"

//컴퓨트쉐이더에서 0 ~ 2 3가지는 사용중
#define MTRL_SCALAR_INT_TILESET_IDX MTRLDATA_PARAM_SCALAR(INT, 3)

#define MTRL_SCALAR_VEC2_MAPSIZE MTRLDATA_PARAM_SCALAR(VEC2, 0)



//단위크기 : 2byte
//총 갯수 : 맵의 가로크기(메가타일) * 세로크기(메가타일)
//HLSL에서는 8개씩 패킹해서 전달(4바이트 int 4개)
#define MXTM_PACK 8u
//MXTM_MAX는 매번 달라지므로 주의(맵사이즈)
struct MXTM
{
	UINT32_4 MXTM_Pack;
};

//단위크기 : 20 byte(dummy) + 2 byte * 16 = 52 byte
//총 갯수 : 4096개
//HLSL에서는 더미 데이터는 버리고 메가타일 정보가 들어있는 나머지 정보만 추출해서 들여올 예정
//2byte * 16 = 32 byte
#define CV5_MAX 4096u
struct CV5
{
	//Megatile
	UINT32_8 MegaTileIndex;
};


//단위크기: 2 byte(비트 플래그) * 16(미니타일의 갯수) = 32byte, 통과가능여부, 높이
//총 갯수 : 65536개
//HLSL에서는 4byte * 8개로 패킹해서 가져올 예정 -> 갯수 = 65536 / 16 = 4096
#define VF4_PACK 16u
#define VF4_MAX 4096u
struct VF4
{
	//32 byte length
	UINT32_8 MiniTileFlags;
};


//단위크기: 2 byte * 16 = 32byte, megatile이 어떤 minitile로 이루어졌는지 표시
//총 갯수 : 65536개
#define VX4_MAX 65536u
struct VX4
{
	UINT32_8 VR4Index;
};

//단위 크기: 1 byte * 64 = 64byte - mini tile각각의 픽셀이 WPE 팔레트의 몇 번 색에 해당하는지를 저장(0 ~ 255 사이의 인덱스가 저장되어있음)
//미니타일은 8 * 8 픽셀임. 그러니까 하나의 미니타일에서 각 타일이 WPE에서 가져와야 하는 색상의 인덱스를 0 ~ 255 사이로 저장하는 것.
//32768개
#define VR4_MAX 32768u
#define VR4_PACK 4u
struct VR4
{
	UINT32_16 ColorPack;
};



//WPE Data : 단위크기 1 byte * 4 = 4 byte (RGBA) - A는 없음(단순 패딩).
//256개
//4Byte만 보낼수는 없으므로 4개 묶음을 하나로 묶어서 보내줘야 한다. -> 256 / 4 = 64
#define WPE_PACK 4u
#define WPE_MAX 64u
struct WPE
{
	UINT32_4 RGBAPack;
};


//0~5까지 데이터는 현재 CCS_SCMapLoader에서 enum 번호로 사용 중이므로 함부로 변경하지 말것
#define e_t_SBUFFER_CV5 REGISTER_IDX(t, 0)
#define e_t_SBUFFER_VX4 REGISTER_IDX(t, 1)
#define e_t_SBUFFER_VF4 REGISTER_IDX(t, 2)
#define e_t_SBUFFER_VR4 REGISTER_IDX(t, 3)
#define e_t_SBUFFER_WPE REGISTER_IDX(t, 4)


#define e_t_SBUFFER_MXTM REGISTER_IDX(t, 5)

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
