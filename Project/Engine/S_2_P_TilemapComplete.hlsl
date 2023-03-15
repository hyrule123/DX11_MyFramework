#include "S_0_H_TilemapComplete.hlsli"

float4 PS_Tilemap_Complete(VS_OUT _in) : SV_TARGET
{
	float4 vOutColor = (float4) 0.f;
	
	if(FALSE == g_CBuffer_Mtrl_Tex.bTEX_0)
		return float4(1.f, 0.f, 1.f, 1.f);
	
	return g_tex_0.Sample(g_Sampler_0, _in.vUV);
}
