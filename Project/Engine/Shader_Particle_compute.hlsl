#include "Shader_header_register.hlsli"
#include "Shader_Func.hlsli"

//=================
//Using Buffers
//=================



//cbuffer CBuffer_SBuffer_SharedInfo : register(b3)
//{
//    tSBufferInfo g_SBufferInfo[eSBUFFER_SHARED_CBUFFER_END];
//}


//cbuffer g_ParticleModuleData : register(b4)
//{
//    float4 vSpawnColor;

//    float4 vSpawnScale;

//    float3 vBoxShapeScale;
//    float fSphereShapeRadius;

//    int SpawnShapeType; // Sphere , Box
//    int SpawnRate; // �ʴ� ���� ����
//    float2 Padding;

//	// Color Change ���
//    float4 vStartColor; // �ʱ� ����
//    float4 vEndColor; // ���� ����

//	// Scale Change ���
//    float4 vStartScale; // �ʱ� ũ��
//    float4 vEndScale; // ���� ũ��	

//	// ���� �ִ�ũ��
//    int iMaxParticleCount;

//	// Module Check(���� int 3��¥�� �迭)
//    int bModule_ParticleSpawn;
//    int bModule_ColorChange;
//    int bModule_ScaleChange;
//}

////RWStructuredBuffer<tParticle> g_SBufferRW_Particle : register(u2);
////RWStructuredBuffer<tRWParticleBuffer> g_SBufferRW_Particle_Shared : register(u3);

#define ePARTICLE_MODULE_PARTICLE_SPAWN 0
#define	ePARTICLE_MODULE_COLOR_CHANGE 1
#define	ePARTICLE_MODULE_SCALE_CHANGE 2


//��ǻƮ���̴��� ����ִ� ���� Mtrl ����ü�� ����ִ°� ����
#define ObjectPos           g_vec4_0

//��ǻƮ���̴��� ����ִ� �������ؽ�ó�� �ػ󵵸� ����
#define NoiseTexResolution  g_vec2_0


#define SpawnCount          g_SBufferRW_Particle_Shared[0].iSpawnCount
#define ParticleMaxCount    g_iMaxParticleCount

#define SpawnModule         g_bSpawn
#define ColorChangeModule   g_bModule_ColorChange
#define ScaleChangeModule   g_bModule_ScaleChange

[numthreads(128, 1, 1)]
void CS_Particle(uint3 _ID : SV_DispatchThreadID )
{
    // ������ ID �� ��ƼŬ���� �ִ� ���� �ѱ��� or ������ ��� ��ƼŬ�� ��Ȱ��ȭ ������ ���
    if ((uint) ParticleMaxCount <= _ID.x)
        return;
        
    tParticle particle = g_SBufferRW_Particle[_ID.x];

           
    if (SpawnModule)
    {
        // ��ƼŬ�� ��Ȱ��ȭ ������ ���
        if (particle.bActive == 0)
        {
            // SpawnCount �� Ȯ��
            // ���� SpawnCount �� 0 �̻��̶��, ��ƼŬ�� Ȱ��ȭ��Ŵ      
            while (0 < SpawnCount)
            {
                int orgvalue = SpawnCount;
                int outvalue = 0;
                InterlockedCompareExchange(SpawnCount, orgvalue, SpawnCount - 1, outvalue);
            
                if (orgvalue == outvalue)
                {
                    particle.bActive = 1;
                    
                    // ���� ����� ���� ����
                    float3 vOut1 = (float3) 0.f;
                    float3 vOut2 = (float3) 0.f;
                    float3 vOut3 = (float3) 0.f;
                    
                    // ��ü ��ȿ �������� ���̵� 0 ~ 1 �� ����ȭ
                    float fNormalizeThreadID = (float) _ID.x / (float) ParticleMaxCount;
                    GaussianSample(g_Tex_Noise, NoiseTexResolution, fNormalizeThreadID, vOut1);
                    GaussianSample(g_Tex_Noise, NoiseTexResolution, fNormalizeThreadID + 0.1f, vOut2);
                    GaussianSample(g_Tex_Noise, NoiseTexResolution, fNormalizeThreadID + 0.2f, vOut3);
                    
                    // Box ����
                    if (g_eSpawnShapeType == 0)
                    {
                        particle.vLocalPos.xyz = float3(g_vBoxShapeScale.x * vOut1.r - g_vBoxShapeScale.x * 0.5f
                                                      , g_vBoxShapeScale.y * vOut2.r - g_vBoxShapeScale.y * 0.5f
                                                      , g_vBoxShapeScale.z * vOut3.r - g_vBoxShapeScale.z * 0.5f);
                        particle.vWorldPos.xyz = particle.vLocalPos.xyz + ObjectPos.xyz;
                        
                        particle.vWorldScale.xyz = float3(10.f, 10.f, 1.f);
                    }
                    
                    // Sphere ����
                    else if (g_eSpawnShapeType == 1)
                    {
                        
                    }
                    
                    
                    float fRadius = 500.f; //vOut1.r * 200.f;
                    float fAngle = vOut2.r * 2 * 3.1415926535f;
                    //particle.vWorldPos.xyz = float3(fRadius * cos(fAngle), fRadius * sin(fAngle), 100.f);                    
                   
                   
                    particle.fAge = 0.f;
                    particle.fLifeTime = 10.f;
                    break;
                }
            }
        }
    }
           
    
    // ��ƼŬ�� Ȱ��ȭ�� ���
    if (particle.bActive)
    {
        // �ӵ��� ���� ��ƼŬ��ġ �̵�
        if (g_bFollowing == 0)
        {
            particle.vWorldPos += particle.vVelocity * g_DeltaTime;
        }
        else if (g_bFollowing == 1)
        {
            particle.vLocalPos += particle.vVelocity * g_DeltaTime;
            particle.vWorldPos.xyz = particle.vLocalPos.xyz + ObjectPos.xyz;
        }
        
        
        
        // ��ƼŬ�� Age �� �ð��� ������Ŵ
        particle.fAge += g_DeltaTime;
        
        // ��ƼŬ�� ������ ������, �ٽ� ��Ȱ��ȭ ���·� �ǵ���
        if (particle.fLifeTime <= particle.fAge)
        {
            particle.bActive = 0.f;
        }
    }
    
    // ������ ����
    g_SBufferRW_Particle[_ID.x] = particle;
    
        
}