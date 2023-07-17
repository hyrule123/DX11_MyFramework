#include "S_H_SetColor.hlsli"


[numthreads(SETCOLOR_THREADS_X, SETCOLOR_THREADS_Y, SETCOLOR_THREADS_Z)]
void CS_SetColor(uint3 DTid : SV_DispatchThreadID)
{
    g_TexRW_Output[DTid.xy] = float4(g_SBufferRW_Test[DTid.x].rgba);
}
