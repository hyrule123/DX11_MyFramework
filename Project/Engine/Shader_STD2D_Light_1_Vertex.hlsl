#include "Shader_std2D_Light_0_header.hlsli"

VS_OUT VS_std2D_Light(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
	
	output.vWorldPos = mul(float4(_in.vLocalPos, 1.f), g_CBuffer_Mtrl_Scalar.MTRL_SCALAR_MAT_WORLD).xyz;
	output.vSV_Pos = mul(float4(_in.vLocalPos, 1.f), g_CBuffer_Mtrl_Scalar.MTRL_SCALAR_MAT_WVP);
    output.vUV = _in.vUV;
	output.uInstID = _in.uInstID;

    return output;
}