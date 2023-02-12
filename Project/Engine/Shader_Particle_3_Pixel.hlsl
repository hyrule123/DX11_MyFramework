#include "Shader_Particle_0_header.hlsli"


float4 PS_Particle(GS_OUT _in) : SV_Target
{
	if(FALSE == g_CBuffer_MtrlData.bTEX_0)
	{
		return float4(1.f, 0.f, 1.f, 1.f);
	}
	
	return g_tex_0.Sample(g_Sampler_1, _in.vUV);
}