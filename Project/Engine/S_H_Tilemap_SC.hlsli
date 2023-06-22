#ifndef S_0_H_TILEMAP_SC
#define S_0_H_TILEMAP_SC

#ifdef __cplusplus

#else

#include "S_H_Register.hlsli"
#include "S_H_SCMapLoader.hlsli"

#endif

//MXTM은 상위 헤더에 정의되어 있음
StructuredBuffer<tMegaTile> g_SBuffer_MegaTile : register(idx_t_SBUFFER_MEGATILE);
StructuredBuffer<tMiniTile> g_SBuffer_MiniTile : register(idx_t_SBUFFER_MINITILE);



// ============
// VespineGeyser
// ============
// Topology: TriangleList
// Rasterizer: CULL_BACK(백페이스 컬링)
// Blend State: Mask
// Depth Stencil: Less
// Shader Domain: Opaque
// Instancing: False
// Animation: No

//g_CBuffer_matCam에서 사용
//#define MTRL_SCALAR_MAT_VP                     MTRLDATA_PARAM_SCALAR(MAT, 0)

////CBuffer 또는 SBuffer에서 사용
//#define MTRL_SCALAR_MAT_WVP                     MTRLDATA_PARAM_SCALAR(MAT, 0)
//#define MTRL_SCALAR_MAT_WORLD                   MTRLDATA_PARAM_SCALAR(MAT, 1)

////자신의 VP 행렬을 들고 있는 카메라 상수버퍼에서의 인덱스
//#define MTRL_SCALAR_INT_CAMIDX                  MTRLDATA_PARAM_SCALAR(INT, 0)

////STD2D 기반 쉐이더에서 사용. 플래그 지정
//#define MTRL_SCALAR_STD2D_FLAG                  MTRLDATA_PARAM_SCALAR(INT, 1)
//ENUM_BEGIN(eMTRL_SCALAR_STD2D_FLAG, int)
//    ENUM_MEMBER(USE_VP, int, 1 << 0) //TRUE == WVP 행렬로 전달 FALSE == VP 행렬 따로 전달
//    ENUM_MEMBER(USE_ANIM, int, 1<<1)
//    ENUM_MEMBER(USE_PIVOT, int, 1<<2)
//    ENUM_MEMBER(NEED_FLIP_X, int, 1<<3)
//    ENUM_MEMBER(USE_COLOR_KEY, int, 1<<4)
//ENUM_END


//g_CBuffer_SBuffer_ShareData[eCBUFFER_SBUFFER_SHAREDATA_IDX::TILE]
//Data 0 = eTileSet
//Data 1 = Mega Tile Size X
//Data 2 = Mega Tile Size Y
#define TILE_SET iData0
#define MEGA_TILE_SIZE_X  iData1
#define MEGA_TILE_SIZE_Y	iData2


//Texture Info
//1. geyser.bmp
//2. 
//3. 
//4. 
//5. 
//6. 
//7. 
//8. 


struct VS_IN
{
	float3 vLocalPos SEMANTIC(POSITION);
	float2 vUV    SEMANTIC(TEXCOORD);
};

struct VS_OUT
{
	float4 vWorldPos SEMANTIC(SV_Position);
	float2 vUV		SEMANTIC(TEXCOORD);
};


#endif
