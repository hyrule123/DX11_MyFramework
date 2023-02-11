#include "Shader_header_register.hlsli"

#define eLIGHT_DIRECTIONAL  (uint)0	//직사광선
#define eLIGHT_POINT        (uint)1	//점광원
#define eLIGHT_SPOTLIGHT    (uint)2	//스포트라이트

struct VS_IN
{
    float3 vLocalPos : POSITION;
    float2 vUV  : TEXCOORD;
};

struct VS_OUT
{
    float3 vWorldPos : POSITION;
    float2 vUV : TEXCOORD;
    float4 vSV_Pos : SV_Position;
};

// ============================
// Std2DShader
// RasterizerState      : None
// BlendState           : Mask
// DepthStencilState    : Less
//
// Parameter
// g_CBuffer_MtrlData.int_0              : AnimUse
// g_CBuffer_MtrlData.vec2_0             : AnimAtlas LeftTop
// g_CBuffer_MtrlData.vec2_1             : AnimAtlas Slice
//
// g_tex_0              : Output Texture
// g_tex_1              : Normal Texture
// ============================
#define bAnimUse        g_CBuffer_MtrlData.int_0
#define LeftTop         g_CBuffer_MtrlData.vec2_0
#define Slice           g_CBuffer_MtrlData.vec2_1
#define Offset          g_CBuffer_MtrlData.vec2_2
#define CanvasSize        g_CBuffer_MtrlData.vec2_3