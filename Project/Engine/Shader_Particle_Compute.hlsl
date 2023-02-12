#include "Shader_Header_Register.hlsli"
#include "Shader_Func.hlsli"

//=================
//Using Buffers
//=================



//cbuffer CBuffer_SBUFFER_SHARED_DATA : register(b3)
//{
//    tSBufferInfo g_SBufferInfo[eSBUFFER_SHARED_CBUFFER_END];
//}


//cbuffer g_ParticleModuleData : register(b4)
//{
//    float4 vSpawnColor;

//    float4 vSpawnScale;

//    float3 vBoxShapeScale;
//    float fSphereShapeRadius;

//    int eSpawnShapeType; // Sphere , Box
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

RWStructuredBuffer<tParticleTransform> g_SBufferRW_Particle : register(u0);
RWStructuredBuffer<tRWParticleBuffer> g_SBufferRW_Particle_Shared : register(u1);

#define ePARTICLE_MODULE_PARTICLE_SPAWN 0
#define	ePARTICLE_MODULE_COLOR_CHANGE 1
#define	ePARTICLE_MODULE_SCALE_CHANGE 2

//��ǻƮ���̴��� ����ִ� ���� Mtrl ����ü�� ����ִ°� ����





#define SpawnModule         g_CBuffer_ParticleModule.bModule_Spawn
#define ColorChangeModule   g_bModule_ColorChange
#define ScaleChangeModule   g_bModule_ScaleChange

[numthreads(128, 1, 1)]
void CS_Particle(uint3 _ID : SV_DispatchThreadID )
{
    // ������ ID �� ��ƼŬ���� �ִ� ���� �ѱ��� or ������ ��� ��ƼŬ�� ��Ȱ��ȭ ������ ���
    if (g_CBuffer_ParticleModule.iMaxParticleCount <= (int) _ID.x)
        return;
           
    if (g_CBuffer_ParticleModule.bModule_Spawn)
    {
        // ��ƼŬ�� ��Ȱ��ȭ ������ ���
        if (g_SBufferRW_Particle[_ID.x].bActive == 0)
        {
            // SpawnCount �� Ȯ��
            // ���� SpawnCount �� 0 �̻��̶��, ��ƼŬ�� Ȱ��ȭ��Ŵ      
            while (0 < g_SBufferRW_Particle_Shared[0].iSpawnCount)
            {
				int orgvalue = g_SBufferRW_Particle_Shared[0].iSpawnCount;
                int outvalue = 0;
				InterlockedCompareExchange(g_SBufferRW_Particle_Shared[0].iSpawnCount, orgvalue, g_SBufferRW_Particle_Shared[0].iSpawnCount - 1, outvalue);
            
                if (orgvalue == outvalue)
                {
                    g_SBufferRW_Particle[_ID.x].bActive = 1;
                    
                    // ���� ����� ���� ����
                    float3 vOut1 = (float3) 0.f;
                    float3 vOut2 = (float3) 0.f;
                    float3 vOut3 = (float3) 0.f;
                    
                    // ��ü ��ȿ �������� ���̵� 0 ~ 1 �� ����ȭ
					float fNormalizeThreadID = (float) _ID.x / (float) g_CBuffer_ParticleModule.iMaxParticleCount;
					GaussianSample(g_Tex_Noise, TEXTURE_NOISE_RESOLUTION, fNormalizeThreadID, vOut1);
					GaussianSample(g_Tex_Noise, TEXTURE_NOISE_RESOLUTION, fNormalizeThreadID + 0.1f, vOut2);
					GaussianSample(g_Tex_Noise, TEXTURE_NOISE_RESOLUTION, fNormalizeThreadID + 0.2f, vOut3);
                    
                    // Box ����
                    if (g_CBuffer_ParticleModule.eSpawnShapeType == 0)
                    {
                        g_SBufferRW_Particle[_ID.x].vLocalPos.xyz = float3(g_CBuffer_ParticleModule.vBoxShapeScale.x * vOut1.r - g_CBuffer_ParticleModule.vBoxShapeScale.x * 0.5f
                                                      , g_CBuffer_ParticleModule.vBoxShapeScale.y * vOut2.r - g_CBuffer_ParticleModule.vBoxShapeScale.y * 0.5f
                                                      , g_CBuffer_ParticleModule.vBoxShapeScale.z * vOut3.r - g_CBuffer_ParticleModule.vBoxShapeScale.z * 0.5f);
                        g_SBufferRW_Particle[_ID.x].vWorldPos.xyz = g_SBufferRW_Particle[_ID.x].vLocalPos.xyz + OWNER_OBJ_POS.xyz;
                        
                        
                        // ���� ũ�� ���������� ���� ũ��� ���� (Min, Max �� ��ġ�ϸ� ����ũ��)
                        float4 vSpawnScale = g_CBuffer_ParticleModule.vSpawnScaleMin + (g_CBuffer_ParticleModule.vSpawnScaleMax - g_CBuffer_ParticleModule.vSpawnScaleMin) * vOut3.x;
                        g_SBufferRW_Particle[_ID.x].vWorldScale.xyz = vSpawnScale.xyz;
                    }
                    
                    // bModule_AddVelocity ���
                    if (g_CBuffer_ParticleModule.bModule_AddVelocity)
                    {
                        // From Center
                        if (g_CBuffer_ParticleModule.eAddVelocityType == 0)
                        {
                            float3 vVelocity = normalize(g_SBufferRW_Particle[_ID.x].vLocalPos.xyz);
                            g_SBufferRW_Particle[_ID.x].vVelocity.xyz = vVelocity * g_CBuffer_ParticleModule.fSpeed;
                        }
                        
                        // To Center
                        else if (g_CBuffer_ParticleModule.eAddVelocityType == 1)
                        {
                               
                        }
                        
                        // Fixed Direction
                        else
                        {
                            
                        }
                    }
                    
                    // Sphere ����
                    else if (g_CBuffer_ParticleModule.eSpawnShapeType == 1)
                    {
                        float fRadius = 500.f; //vOut1.r * 200.f;
                        float fAngle = vOut2.r * 6.2831852f;
                        //g_SBufferRW_Particle[_ID.x].vWorldPos.xyz = float3(fRadius * cos(fAngle), fRadius * sin(fAngle), 100.f);
                    }
                    
                    
                    g_SBufferRW_Particle[_ID.x].vColor = g_CBuffer_ParticleModule.vSpawnColor;
                                      
                    g_SBufferRW_Particle[_ID.x].fAge = 0.f;
                    g_SBufferRW_Particle[_ID.x].fLifeTime = g_CBuffer_ParticleModule.fMinLifeTime + (g_CBuffer_ParticleModule.fMaxLifeTime - g_CBuffer_ParticleModule.fMinLifeTime) * vOut2.r;
                    break;
                }
            }
        }
    }
           
    
    // ��ƼŬ�� Ȱ��ȭ�� ���
    if (1 == g_SBufferRW_Particle[_ID.x].bActive)
    {
        // ��ƼŬ�� fAge �� �ð��� ������Ŵ
        g_SBufferRW_Particle[_ID.x].fAge += g_CBuffer_GlobalData.fDeltaTime;
        g_SBufferRW_Particle[_ID.x].fNormalizedAge = saturate(g_SBufferRW_Particle[_ID.x].fAge / g_SBufferRW_Particle[_ID.x].fLifeTime);
        
        // ��ƼŬ�� ������ ������, �ٽ� ��Ȱ��ȭ ���·� �ǵ���
        if (g_SBufferRW_Particle[_ID.x].fLifeTime <= g_SBufferRW_Particle[_ID.x].fAge)
        {
            g_SBufferRW_Particle[_ID.x].bActive = 0;
        }
        
        
        // �ӵ� ����(bModule_Drag) ���
        if (g_CBuffer_ParticleModule.bModule_Drag)
        {
            // ��ƼŬ�� ���� �ӷ�
            float fSpeed = length(g_SBufferRW_Particle[_ID.x].vVelocity);
            float fDrag = g_CBuffer_ParticleModule.fStartDrag + (g_CBuffer_ParticleModule.fEndDrag - g_CBuffer_ParticleModule.fStartDrag) * g_SBufferRW_Particle[_ID.x].fNormalizedAge;
            
            if (fDrag < fSpeed)
            {
                g_SBufferRW_Particle[_ID.x].vVelocity = normalize(g_SBufferRW_Particle[_ID.x].vVelocity) * fDrag;
            }
        }
        
        
        
        // �ӵ��� ���� ��ƼŬ��ġ �̵�
        if (g_CBuffer_ParticleModule.bFollowing == 0)
        {
            g_SBufferRW_Particle[_ID.x].vWorldPos += g_SBufferRW_Particle[_ID.x].vVelocity * g_CBuffer_GlobalData.fDeltaTime;
        }
        else if (g_CBuffer_ParticleModule.bFollowing == 1)
        {
            g_SBufferRW_Particle[_ID.x].vLocalPos += g_SBufferRW_Particle[_ID.x].vVelocity * g_CBuffer_GlobalData.fDeltaTime;
            g_SBufferRW_Particle[_ID.x].vWorldPos.xyz = g_SBufferRW_Particle[_ID.x].vLocalPos.xyz + OWNER_OBJ_POS.xyz;
        }
        
        
        // ũ�� ��ȭ ����� Ȱ��ȭ �Ǿ�������
        if (g_CBuffer_ParticleModule.bModule_ScaleChange)
            g_SBufferRW_Particle[_ID.x].fScaleFactor = g_CBuffer_ParticleModule.fStartScale + g_SBufferRW_Particle[_ID.x].fNormalizedAge * (g_CBuffer_ParticleModule.fEndScale - g_CBuffer_ParticleModule.fStartScale);
        else
            g_SBufferRW_Particle[_ID.x].fScaleFactor = 1.f;
        
        
        // ���� ��ȭ����� Ȱ��ȭ �Ǿ�������
        if (g_CBuffer_ParticleModule.bModule_ColorChange)
        {
            g_SBufferRW_Particle[_ID.x].vColor = g_CBuffer_ParticleModule.vStartColor + g_SBufferRW_Particle[_ID.x].fNormalizedAge * (g_CBuffer_ParticleModule.vEndColor - g_CBuffer_ParticleModule.vStartColor);
        }
        
    }

    
    
}