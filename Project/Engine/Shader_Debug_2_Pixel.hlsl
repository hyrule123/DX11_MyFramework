#include "Shader_Debug_0_Header.hlsli"

float4 PS_Debug(VS_OUT _in) : SV_TARGET
{
	float4 vColor = SHADER_DEBUG_vecColor;
	
	return vColor;
}