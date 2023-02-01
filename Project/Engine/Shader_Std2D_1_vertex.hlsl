
#include "Shader_std2d_0_header.hlsli"

VS_OUT VS_std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
	
    output.vPosSV = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
	
	return output;
}