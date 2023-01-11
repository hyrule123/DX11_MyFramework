#include "Shader_header_register.hlsli"

#define Debug_matWVP g_mat_0
#define Debug_vecColor g_vec4_0

struct VS_IN
{
    float3 vPos : POSITION; // semantic  
};

struct VS_OUT
{
    float4 vPos : SV_Position;
};
