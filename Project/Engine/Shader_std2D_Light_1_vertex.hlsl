#include "Shader_std2D_Light_0_header.hlsli"

VS_OUT VS_std2D_Light(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
	
    output.vSV_Pos = float4(_in.vPos, 1.f);
    output.vUV = _in.vUV;

    return output;
}