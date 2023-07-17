#ifndef S_H_INITIALIZE
#define S_H_INITIALIZE

#include "S_H_Register.hlsli"

#define REGISLOT_u_INIT_SETTING			REGISTER_SLOT(u, 0)	//처음에 최초 한번 작동해서 시스템 정보 확인

#define INIT_THREADS_X 1
#define INIT_THREADS_Y 1
#define INIT_THREADS_Z 1

#ifndef __cplusplus
StructuredBuffer<tInitSetting> g_SBuffer_InitSettings : register(REGISLOT_t_INIT_SETTING);
RWStructuredBuffer<tInitSetting> g_SBufferRW_InitSetting : register(REGISLOT_u_INIT_SETTING);
#endif

#endif
