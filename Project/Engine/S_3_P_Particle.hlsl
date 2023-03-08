#include "S_0_H_Particle.hlsli"


float4 PS_Particle(GS_OUT _in) : SV_Target
{
	if(FALSE == g_CBuffer_Mtrl_Tex.bTEX_0)
	{
		return float4(1.f, 0.f, 1.f, 1.f);
	}
	
	return g_tex_0.Sample(g_Sampler_1, _in.vUV);
}