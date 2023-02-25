#include "Shader_Debug_0_Header.hlsli"

float4 PS_Debug(VS_OUT _in) : SV_TARGET
{
	tMtrlScalarData Data = GetMtrlScalarData(_in.uInstID);
	
	float4 vColor = Data.MTRL_SCALAR_DEBUG_VEC4_COLOR;
	
	return vColor;
}