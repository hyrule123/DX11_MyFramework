#include "S_H_SetColor.hlsli"


RWTexture2D<float4> g_TexRW_Output : register(REGISLOT_u_TEXTURERW_SETCOLOR);
RWStructuredBuffer<float4> g_SBufferRW_Test : register(REGISLOT_u_SBUFFERRW_SETCOLOR);

[numthreads(SETCOLOR_THREADS_X, SETCOLOR_THREADS_Y, SETCOLOR_THREADS_Z)]
void CS_SetColor(uint3 DTid : SV_DispatchThreadID)
{
    g_TexRW_Output[DTid.xy] = float4(g_SBufferRW_Test[DTid.x].rgba);
}
