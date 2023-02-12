#include "Shader_header_register.hlsli"

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
// g_CBuffer_MtrlData.INT_0              : AnimUse
// g_CBuffer_MtrlData.VEC2_0             : AnimAtlas LeftTop
// g_CBuffer_MtrlData.VEC2_1             : AnimAtlas Slice
//
// g_tex_0              : Output Texture
// g_tex_1              : Normal Texture
// ============================
#define bAnimUse        g_CBuffer_MtrlData.INT_0
#define LeftTop         g_CBuffer_MtrlData.VEC2_0
#define Slice           g_CBuffer_MtrlData.VEC2_1
#define Offset          g_CBuffer_MtrlData.VEC2_2
#define CanvasSize        g_CBuffer_MtrlData.VEC2_3