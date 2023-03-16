#include "S_H_Register.hlsli"


RWTexture2D<float4> g_TexRW_Output : register(idx_u_SETCOLOR_TEXTURERW);
RWStructuredBuffer<float4> g_SBufferRW_Test : register(idx_u_SETCOLOR_SBUFFERRW);

[numthreads(32, 32, 1)]
void CS_SetColor(uint3 DTid : SV_DispatchThreadID)
{
    g_TexRW_Output[DTid.xy] = float4(g_SBufferRW_Test[DTid.x].rgba);
}