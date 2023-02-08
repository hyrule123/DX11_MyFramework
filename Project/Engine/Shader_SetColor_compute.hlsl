#include "Shader_header_register.hlsli"



#define Color g_vec4_0


[numthreads(32, 32, 1)]
void CS_SetColor(uint3 DTid : SV_DispatchThreadID)
{
    g_Output[DTid.xy] = float4(g_TestSBUffer[DTid.x].rgba);
}