#ifndef S_H_SETCOLOR
#define S_H_SETCOLOR

#include "S_H_Register.hlsli"

#define REGISLOT_t_SBUFFER_SETCOLOR			REGISTER_SLOT(t, 15)

#define REGISLOT_u_TEXTURERW_SETCOLOR			REGISTER_SLOT(u, 0)
#define REGISLOT_u_SBUFFERRW_SETCOLOR			REGISTER_SLOT(u, 1)


#define SETCOLOR_THREADS_X 32
#define SETCOLOR_THREADS_Y 32
#define SETCOLOR_THREADS_Z 1

#ifndef __cplusplus
StructuredBuffer<float4>	g_SBuffer_SetColor : register(REGISLOT_t_SBUFFER_SETCOLOR);
RWTexture2D<float4>			g_TexRW_Output : register(REGISLOT_u_TEXTURERW_SETCOLOR);
RWStructuredBuffer<float4>	g_SBufferRW_Test : register(REGISLOT_u_SBUFFERRW_SETCOLOR);
#endif

#endif
