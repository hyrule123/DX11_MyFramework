#include "Shader_std2D_Light_0_header.hlsli"

VS_OUT VS_std2D_Light(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
	
    output.vWorldPos = mul(float4(_in.vLocalPos, 1.f), g_matWorld).xyz;
    output.vSV_Pos = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;

    return output;
}