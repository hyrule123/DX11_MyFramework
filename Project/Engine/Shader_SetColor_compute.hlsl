#include "Shader_header_register.hlsli"

#define Color g_CBuffer_Transform.vec4_0


RWTexture2D<float4> g_TexRW_Output : register(u0);
RWStructuredBuffer<float4> g_SBufferRW_Test : register(u1);

[numthreads(32, 32, 1)]
void CS_SetColor(uint3 DTid : SV_DispatchThreadID)
{
    g_TexRW_Output[DTid.xy] = float4(g_SBufferRW_Test[DTid.x].rgba);
}