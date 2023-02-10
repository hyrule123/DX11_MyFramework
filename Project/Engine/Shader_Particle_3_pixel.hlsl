#include "Shader_Particle_0_header.hlsli"


float4 PS_Particle(GS_OUT _in) : SV_Target
{
	if(Padding0 == 1)
        return float4(0.f, 1.f, 0.f, 1.f);
	
	return float4(1.0f, 0.f, 0.f, 1.0f);
}