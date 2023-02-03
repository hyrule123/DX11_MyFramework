#include "Shader_header_register.hlsli"

RWTexture2D<float4> g_Output : register(u0);

RWStructuredBuffer<float4> g_TestSBUffer : register(u1);

#define Color g_vec4_0


[numthreads(32, 32, 1)]
void CS_SetColor(uint3 DTid : SV_DispatchThreadID)
{
    //g_Output[DTid.xy] = float4(Color.xyz, 1.f);
    
    g_Output[DTid.xy] = float4(g_TestSBUffer[DTid.x].rgba);
}