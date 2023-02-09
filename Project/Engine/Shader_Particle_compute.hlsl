#include "Shader_header_register.hlsli"


#define ePARTICLE_MODULE_PARTICLE_SPAWN 0
#define	ePARTICLE_MODULE_COLOR_CHANGE 1
#define	ePARTICLE_MODULE_SCALE_CHANGE 2

//RWStructuredBuffer<tParticle> g_ParticleSBuffer : register(u2);
//RWStructuredBuffer<tRWParticleBuffer> g_ParticleSBufferSharedRW : register(u3);

#define spawnCount g_ParticleSBufferSharedRW[0].SpawnCount

[numthreads(128, 1, 1)]
void CS_Particle(uint3 _ID : SV_DispatchThreadID )
{
    if (g_SBufferInfo[eSBUFFER_SHARED_CBUFFER_PARTICLE].g_uSBufferCount <= _ID.x)
        return;
    
    //������ Ȱ��ȭ������ ���
    if(g_ParticleSBuffer[_ID.x].LifeTime > 0.f)
    {
        //�̵� ó��
        g_ParticleSBuffer[_ID.x].vWorldPos += g_ParticleSBuffer[_ID.x].vVelocity * g_DeltaTime * 0.1f;
        
        //DeltaTime��ŭ ������ �����ְ� ������ ������ �� Age�� -500���� �ٿ���
        g_ParticleSBuffer[_ID.x].Age += g_DeltaTime;
        if (g_ParticleSBuffer[_ID.x].Age > g_ParticleSBuffer[_ID.x].LifeTime)
        {
            g_ParticleSBuffer[_ID.x].Age = -1500.f;
        }
    }
    //��Ȱ��ȭ ���¿� �� ��ƼŬ�ε� ���� SpawnCount�� ���� ���� ���
    else if(g_ParticleSBuffer[_ID.x].Age < -1000.f)
    {
        while (0 < spawnCount)
        {
            int origVal = spawnCount;
            int outVal = 0;
            
            InterlockedCompareExchange(spawnCount, origVal, spawnCount - 1, outVal);
            
            if (spawnCount == outVal)
            {
                g_ParticleSBuffer[_ID.x].Age = g_ParticleSBuffer[_ID.x].LifeTime;
            }
            
            
        }
        
    }
    
            
}