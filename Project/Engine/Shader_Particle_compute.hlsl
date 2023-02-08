#include "Shader_header_register.hlsli"


#define ePARTICLE_MODULE_PARTICLE_SPAWN 0
#define	ePARTICLE_MODULE_COLOR_CHANGE 1
#define	ePARTICLE_MODULE_SCALE_CHANGE 2



[numthreads(128, 1, 1)]
void CS_Particle(uint3 _ID : SV_DispatchThreadID )
{
    if (g_SBufferInfo[eSBUFFER_SHARED_CBUFFER_PARTICLE].g_uSBufferCount <= _ID.x)
        return;
    
    g_ParticleSBuffer[_ID.x].vWorldPos += g_ParticleSBuffer[_ID.x].vVelocity * g_DeltaTime * 0.1f;
}