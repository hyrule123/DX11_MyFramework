#include "Shader_Tilemap_0_header.hlsli"

VTX_TILEMAP_OUT VS_Tilemap(VTX_TILEMAP_IN _in)
{
    VTX_TILEMAP_OUT output = (VTX_TILEMAP_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV * float2(g_int_0, g_int_1); // 전체 UV 를 타일 개수만큼 확장 
    
    return output;
}