#ifndef SC_TILEMAP_LOADER_HLSLI
#define SC_TILEMAP_LOADER_HLSLI

#include "DefaultShader/CommonStruct.hlsli"
#include "DefaultShader/Resource.hlsli"

//단위크기 : 2byte
//총 갯수 : 맵의 가로크기(메가타일) * 세로크기(메가타일)
//HLSL에서는 8개씩 패킹해서 전달(4바이트 int 4개)
#define MXTM_PACK 8u
//MXTM_MAX는 매번 달라지므로 주의(맵사이즈)
struct tMXTM
{
	uint4 MXTM_Pack;
};

//단위크기 : 20 byte(dummy) + 2 byte * 16 = 52 byte
//총 갯수 : 4096개
//HLSL에서는 더미 데이터는 버리고 메가타일 정보가 들어있는 나머지 정보만 추출해서 들여올 예정
//2byte * 16 = 32 byte
#define CV5_MAX 2048u
struct tCV5
{
	uint TerrainType;
	uint Flags;
	float2 Padding;
	
	
	
	//Megatile
	uintx8 MegaTileIndex;
};


//단위크기: 2 byte(비트 플래그) * 16(미니타일의 갯수) = 32byte, 통과가능여부, 높이
//총 갯수 : 65536개
//HLSL에서는 4byte * 8개로 패킹해서 가져올 예정 -> 갯수 = 65536
#define VF4_PACK 16u
#define VF4_MAX 65536u
struct tVF4
{
	//32 byte length
	uintx8 MiniTileFlags;
};


//단위크기: 2 byte * 16 = 32byte, megatile이 어떤 minitile로 이루어졌는지 표시
//총 갯수 : 65536개
#define VX4_MAX 65536u
struct tVX4
{
	//VR4에서의 인덱스를 저장함(변수명 틀린거 아님)
	uintx8 VR4Index;
};

//단위 크기: 1 byte * 64 = 64byte - mini tile각각의 픽셀이 WPE 팔레트의 몇 번 색에 해당하는지를 저장(0 ~ 255 사이의 인덱스가 저장되어있음)
//미니타일은 8 * 8 픽셀임. 그러니까 하나의 미니타일에서 각 타일이 WPE에서 가져와야 하는 색상의 인덱스를 0 ~ 255 사이로 저장하는 것.
//32768개
#define VR4_MAX 32768u
#define VR4_PACK 4u
struct tVR4
{
	uintx16 ColorPack;
};



//WPE Data : 단위크기 1 byte * 4 = 4 byte (RGBA) - A는 없음(단순 패딩).
//256개
//4Byte만 보낼수는 없으므로 4개 묶음을 하나로 묶어서 보내줘야 한다. -> 256 / 4 = 64
#define WPE_PACK 4u
#define WPE_MAX 64u
struct tWPE
{
	uint4 RGBAPack;
};

struct tMegaTile
{
	BOOL bBuildUnable;
	float3 Padding;
};

struct tMiniTile
{
	//0 = Low, 1 = Mid, 2 = High
	BOOL bWalkable;
	uint uFloor;
    BOOL bIsRamp;
	float Padding;
};

SBUFFER(g_SBuffer_CV5, tCV5, t, 0);
SBUFFER(g_SBuffer_VX4, tVX4, t, 1);
SBUFFER(g_SBuffer_VF4, tVF4, t, 2);
SBUFFER(g_SBuffer_VR4, tVR4, t, 3);
SBUFFER(g_SBuffer_WPE, tWPE, t, 4);

SBUFFER(g_SBuffer_MXTM, tMXTM, t, 9);
SBUFFER(g_MegaTile, tMegaTile, t, 10);
SBUFFER(g_MiniTile, tMiniTile, t, 11);

TEXTURE2D(g_TargetSCTilemapTexture, t, 6);

TEXTURE2D_RW(g_TargetSCTilemapTextureRW, u, 0, float4);
SBUFFER_RW(g_MegaTileRW, tMegaTile, u, 1);
SBUFFER_RW(g_MiniTileRW, tMiniTile, u, 2);

#endif //SC_TILEMAP_LOADER_HLSLI
