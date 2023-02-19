#include "Shader_Debug_0_Header.hlsli"

VS_OUT VS_Debug( VS_IN _in )
{
    VS_OUT _out = (VS_OUT) 0.f;
	
	_out.vPos = mul(float4(_in.vPos, 1.f), g_CBuffer_Mtrl_Scalar.SHADER_DEBUG_matWVP);
	
	return _out;
}