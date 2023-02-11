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

//    int eSpawnShapeType; // Sphere , Box
//    int SpawnRate; // 초당 생성 개수
//    float2 Padding;

//	// Color Change 모듈
//    float4 vStartColor; // 초기 색상
//    float4 vEndColor; // 최종 색상

//	// Scale Change 모듈
//    float4 vStartScale; // 초기 크기
//    float4 vEndScale; // 최종 크기	

//	// 버퍼 최대크기
//    int iMaxParticleCount;

//	// Module Check(현재 int 3개짜리 배열)
//    int bModule_ParticleSpawn;
//    int bModule_ColorChange;
//    int bModule_ScaleChange;
//}

RWStructuredBuffer<tParticleTransform> g_SBufferRW_Particle : register(u0);
RWStructuredBuffer<tRWParticleBuffer> g_SBufferRW_Particle_Shared : register(u1);

#define ePARTICLE_MODULE_PARTICLE_SPAWN 0
#define	ePARTICLE_MODULE_COLOR_CHANGE 1
#define	ePARTICLE_MODULE_SCALE_CHANGE 2

//컴퓨트쉐이더가 들고있는 개별 Mtrl 구조체에 들어있는값 참조
#define OWNER_OBJ_POS g_CBuffer_MtrlData.vec4_0

//컴퓨트쉐이더가 들고있는 노이즈텍스처의 해상도를 저장
#define NoiseTexResolution  g_CBuffer_MtrlData.vec2_0

#define SpawnCount          g_SBufferRW_Particle_Shared[0].iSpawnCount
#define ParticleMaxCount    g_CBuffer_ParticleModule.iMaxParticleCount

#define SpawnModule         g_CBuffer_ParticleModule.bSpawn
#define ColorChangeModule   g_bModule_ColorChange
#define ScaleChangeModule   g_bModule_ScaleChange

[numthreads(128, 1, 1)]
void CS_Particle(uint3 _ID : SV_DispatchThreadID )
{
    // 스레드 ID 가 파티클버퍼 최대 수를 넘긴경우 or 스레드 담당 파티클이 비활성화 상태인 경우
    if (ParticleMaxCount <= (int) _ID.x)
        return;
           
    if (SpawnModule)
    {
        // 파티클이 비활성화 상태인 경우
        if (g_SBufferRW_Particle[_ID.x].bActive == 0)
        {
            // SpawnCount 를 확인
            // 만약 SpawnCount 가 0 이상이라면, 파티클을 활성화시킴      
            while (0 < SpawnCount)
            {
                int orgvalue = SpawnCount;
                int outvalue = 0;
                InterlockedCompareExchange(SpawnCount, orgvalue, SpawnCount - 1, outvalue);
            
                if (orgvalue == outvalue)
                {
                    g_SBufferRW_Particle[_ID.x].bActive = 1;
                    
                    // 랜덤 결과를 받을 변수
                    float3 vOut1 = (float3) 0.f;
                    float3 vOut2 = (float3) 0.f;
                    float3 vOut3 = (float3) 0.f;
                    
                    // 전체 유효 스레드의 아이디를 0 ~ 1 로 정규화
                    float fNormalizeThreadID = (float) _ID.x / (float) ParticleMaxCount;
                    GaussianSample(g_Tex_Noise, NoiseTexResolution, fNormalizeThreadID, vOut1);
                    GaussianSample(g_Tex_Noise, NoiseTexResolution, fNormalizeThreadID + 0.1f, vOut2);
                    GaussianSample(g_Tex_Noise, NoiseTexResolution, fNormalizeThreadID + 0.2f, vOut3);
                    
                    // Box 스폰
                    if (g_CBuffer_ParticleModule.eSpawnShapeType == 0)
                    {
                        g_SBufferRW_Particle[_ID.x].vLocalPos.xyz = float3(g_CBuffer_ParticleModule.vBoxShapeScale.x * vOut1.r - g_CBuffer_ParticleModule.vBoxShapeScale.x * 0.5f
                                                      , g_CBuffer_ParticleModule.vBoxShapeScale.y * vOut2.r - g_CBuffer_ParticleModule.vBoxShapeScale.y * 0.5f
                                                      , g_CBuffer_ParticleModule.vBoxShapeScale.z * vOut3.r - g_CBuffer_ParticleModule.vBoxShapeScale.z * 0.5f);
                        g_SBufferRW_Particle[_ID.x].vWorldPos.xyz = g_SBufferRW_Particle[_ID.x].vLocalPos.xyz + OWNER_OBJ_POS.xyz;
                        
                        
                        // 스폰 크기 범위내에서 랜덤 크기로 지정 (Min, Max 가 일치하면 고정크기)
                        float4 vSpawnScale = g_CBuffer_ParticleModule.vSpawnScaleMin + (g_CBuffer_ParticleModule.vSpawnScaleMax - g_CBuffer_ParticleModule.vSpawnScaleMin) * vOut3.x;
                        g_SBufferRW_Particle[_ID.x].vWorldScale.xyz = vSpawnScale.xyz;
                    }
                    
                    // bAddVelocity 모듈
                    if (g_CBuffer_ParticleModule.bAddVelocity)
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
                    
                    // Sphere 스폰
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
           
    
    // 파티클이 활성화인 경우
    if (g_SBufferRW_Particle[_ID.x].bActive)
    {
        // 파티클의 fAge 에 시간을 누적시킴
        g_SBufferRW_Particle[_ID.x].fAge += g_CBuffer_GlobalData.fDeltaTime;
        g_SBufferRW_Particle[_ID.x].fNormalizedAge = saturate(g_SBufferRW_Particle[_ID.x].fAge / g_SBufferRW_Particle[_ID.x].fLifeTime);
        
        // 파티클의 수명이 끝나면, 다시 비활성화 상태로 되돌림
        if (g_SBufferRW_Particle[_ID.x].fLifeTime <= g_SBufferRW_Particle[_ID.x].fAge)
        {
            g_SBufferRW_Particle[_ID.x].bActive = 0.f;
        }
        
        
        // 속도 제한(bDrag) 모듈
        if (g_CBuffer_ParticleModule.bDrag)
        {
            // 파티클의 현재 속력
            float fSpeed = length(g_SBufferRW_Particle[_ID.x].vVelocity);
            float fDrag = g_CBuffer_ParticleModule.fStartDrag + (g_CBuffer_ParticleModule.fEndDrag - g_CBuffer_ParticleModule.fStartDrag) * g_SBufferRW_Particle[_ID.x].fNormalizedAge;
            
            if (fDrag < fSpeed)
            {
                g_SBufferRW_Particle[_ID.x].vVelocity = normalize(g_SBufferRW_Particle[_ID.x].vVelocity) * fDrag;
            }
        }
        
        
        
        // 속도에 따른 파티클위치 이동
        if (g_CBuffer_ParticleModule.bFollowing == 0)
        {
            g_SBufferRW_Particle[_ID.x].vWorldPos += g_SBufferRW_Particle[_ID.x].vVelocity * g_CBuffer_GlobalData.fDeltaTime;
        }
        else if (g_CBuffer_ParticleModule.bFollowing == 1)
        {
            g_SBufferRW_Particle[_ID.x].vLocalPos += g_SBufferRW_Particle[_ID.x].vVelocity * g_CBuffer_GlobalData.fDeltaTime;
            g_SBufferRW_Particle[_ID.x].vWorldPos.xyz = g_SBufferRW_Particle[_ID.x].vLocalPos.xyz + OWNER_OBJ_POS.xyz;
        }
        
        
        // 크기 변화 모듈이 활성화 되어있으면
        if (g_CBuffer_ParticleModule.bScaleChange)
            g_SBufferRW_Particle[_ID.x].fScaleFactor = g_CBuffer_ParticleModule.fStartScale + g_SBufferRW_Particle[_ID.x].fNormalizedAge * (g_CBuffer_ParticleModule.fEndScale - g_CBuffer_ParticleModule.fStartScale);
        else
            g_SBufferRW_Particle[_ID.x].fScaleFactor = 1.f;
        
        
        // 색상 변화모듈이 활성화 되어있으면
        if (g_CBuffer_ParticleModule.bColorChange)
        {
            g_SBufferRW_Particle[_ID.x].vColor = g_CBuffer_ParticleModule.vStartColor + g_SBufferRW_Particle[_ID.x].fNormalizedAge * (g_CBuffer_ParticleModule.vEndColor - g_CBuffer_ParticleModule.vStartColor);
        }
        
    }

    
    
}