#include "Shader_Header_Register.hlsli"
#include "Shader_Func.hlsli"

RWStructuredBuffer<tParticleTransform> g_SBufferRW_Particle : register(u0);
RWStructuredBuffer<tRWParticleBuffer> g_SBufferRW_Particle_Shared : register(u1);

#define ePARTICLE_MODULE_PARTICLE_SPAWN 0
#define	ePARTICLE_MODULE_COLOR_CHANGE 1
#define	ePARTICLE_MODULE_SCALE_CHANGE 2


void PModule_Spawn(uint _uID);
void PModule_Drag(uint _uID);

[numthreads(128, 1, 1)]
void CS_Particle(uint3 _ID : SV_DispatchThreadID )
{
    // 스레드 ID 가 파티클버퍼 최대 수를 넘긴경우 or 스레드가 담당하고있는 파티클이 비활성화 상태인 경우
    if (g_CBuffer_ParticleModule.iMaxParticleCount <= (int) _ID.x)
        return;
    
    if (TRUE == g_CBuffer_ParticleModule.bModule_Spawn)
    {
		PModule_Spawn(_ID.x);
    }
           
    
    // 파티클이 활성화인 경우
    if (TRUE == g_SBufferRW_Particle[_ID.x].bActive)
    {
        
        //우선 파티클 업데이트
		// 파티클의 fAge 에 시간을 누적시킴
		g_SBufferRW_Particle[_ID.x].fAge += g_CBuffer_GlobalData.fDeltaTime;
		g_SBufferRW_Particle[_ID.x].fNormalizedAge = saturate(g_SBufferRW_Particle[_ID.x].fAge / g_SBufferRW_Particle[_ID.x].fLifeTime);
        
        // 파티클의 수명이 끝나면 다시 비활성화 상태로 되돌린다.
		if (g_SBufferRW_Particle[_ID.x].fLifeTime <= g_SBufferRW_Particle[_ID.x].fAge)
		{
			g_SBufferRW_Particle[_ID.x].bActive = FALSE;
			return;
		}
        
        
        
		if (TRUE == g_CBuffer_ParticleModule.bModule_Rotation)
		{
			g_SBufferRW_Particle[_ID.x].vWorldRotation += float4(g_CBuffer_ParticleModule.vRotRadPerSec * g_CBuffer_GlobalData.fDeltaTime, 0.f);
		}
        
        
        // 속도 제한(bModule_Drag) 모듈
		if (TRUE == g_CBuffer_ParticleModule.bModule_Drag)
		{
			PModule_Drag(_ID.x);
		}
        
        
        // Drag 모듈로부터 계산된 속도에 따른 파티클위치 이동
        if (g_CBuffer_ParticleModule.bFollowing == 0)
        {
            g_SBufferRW_Particle[_ID.x].vWorldPos += g_SBufferRW_Particle[_ID.x].vVelocity * g_CBuffer_GlobalData.fDeltaTime;
        }
        else if (g_CBuffer_ParticleModule.bFollowing == 1)
        {
            g_SBufferRW_Particle[_ID.x].vLocalPos += g_SBufferRW_Particle[_ID.x].vVelocity * g_CBuffer_GlobalData.fDeltaTime;
            g_SBufferRW_Particle[_ID.x].vWorldPos.xyz = g_SBufferRW_Particle[_ID.x].vLocalPos.xyz + OWNER_OBJ_POS.xyz;
        }
        
        
        
        // 크기 변화 모듈이 활성화 되어있으면 시간이 지남에 따라 ScaleFactor를 변화시켜준다.
        if (g_CBuffer_ParticleModule.bModule_ScaleChange)
            g_SBufferRW_Particle[_ID.x].fScaleFactor = g_CBuffer_ParticleModule.fStartScale + g_SBufferRW_Particle[_ID.x].fNormalizedAge * (g_CBuffer_ParticleModule.fEndScale - g_CBuffer_ParticleModule.fStartScale);
        else
            g_SBufferRW_Particle[_ID.x].fScaleFactor = 1.f;
        
        
        
        // 색상 변화모듈이 활성화 되어있으면 시간이 지남에 따라 색상을 변화시켜줌.
        if (g_CBuffer_ParticleModule.bModule_ColorChange)
        {
            g_SBufferRW_Particle[_ID.x].vColor = g_CBuffer_ParticleModule.vStartColor + g_SBufferRW_Particle[_ID.x].fNormalizedAge * (g_CBuffer_ParticleModule.vEndColor - g_CBuffer_ParticleModule.vStartColor);
        }
        
    }

    
    
}


void PModule_Spawn(uint _uID)
{
            // 파티클이 비활성화 상태인 경우 도로 살려내는 작업 시작
	if (FALSE == g_SBufferRW_Particle[_uID].bActive)
	{
        // SpawnCount 를 확인
        // 만약 SpawnCount 가 0 이상이라면, 파티클을 활성화시킴      
		while (0 < g_SBufferRW_Particle_Shared[0].iSpawnCount)
		{
			int orgvalue = g_SBufferRW_Particle_Shared[0].iSpawnCount;
			int outvalue = 0;
			InterlockedCompareExchange(g_SBufferRW_Particle_Shared[0].iSpawnCount, orgvalue, g_SBufferRW_Particle_Shared[0].iSpawnCount - 1, outvalue);
            
			if (orgvalue == outvalue)
			{
				g_SBufferRW_Particle[_uID].bActive = 1;
                    
                    // 랜덤 결과를 받을 변수
				float3 vOut1 = (float3) 0.f;
				float3 vOut2 = (float3) 0.f;
				float3 vOut3 = (float3) 0.f;
                    
                    // 전체 유효 스레드의 아이디를 0 ~ 1 로 정규화
				float fNormalizeThreadID = (float) _uID / (float) g_CBuffer_ParticleModule.iMaxParticleCount;
				GaussianSample(g_Tex_Noise, TEXTURE_NOISE_RESOLUTION, fNormalizeThreadID, vOut1);
				GaussianSample(g_Tex_Noise, TEXTURE_NOISE_RESOLUTION, fNormalizeThreadID + 0.1f, vOut2);
				GaussianSample(g_Tex_Noise, TEXTURE_NOISE_RESOLUTION, fNormalizeThreadID + 0.2f, vOut3);
                    
                // Box 스폰
				if (g_CBuffer_ParticleModule.eSpawnShapeType == 0)
				{
					g_SBufferRW_Particle[_uID].vLocalPos.xyz = float3(g_CBuffer_ParticleModule.vBoxShapeScale.x * vOut1.r - g_CBuffer_ParticleModule.vBoxShapeScale.x * 0.5f
                                                      , g_CBuffer_ParticleModule.vBoxShapeScale.y * vOut2.r - g_CBuffer_ParticleModule.vBoxShapeScale.y * 0.5f
                                                      , g_CBuffer_ParticleModule.vBoxShapeScale.z * vOut3.r - g_CBuffer_ParticleModule.vBoxShapeScale.z * 0.5f);
					g_SBufferRW_Particle[_uID].vWorldPos.xyz = g_SBufferRW_Particle[_uID].vLocalPos.xyz + OWNER_OBJ_POS.xyz;
                        
                        
                        // 스폰 크기 범위내에서 랜덤 크기로 지정 (Min, Max 가 일치하면 고정크기)
					float4 vSpawnScale = g_CBuffer_ParticleModule.vSpawnScaleMin + (g_CBuffer_ParticleModule.vSpawnScaleMax - g_CBuffer_ParticleModule.vSpawnScaleMin) * vOut3.x;
					g_SBufferRW_Particle[_uID].vWorldScale.xyz = vSpawnScale.xyz;
				}
               
                
                
                
                // bModule_AddVelocity 모듈
				if (TRUE == g_CBuffer_ParticleModule.bModule_AddVelocity)
				{
                        // From Center
					if (FALSE == g_CBuffer_ParticleModule.eAddVelocityType)
					{
						float3 vVelocity = normalize(g_SBufferRW_Particle[_uID].vLocalPos.xyz);
						g_SBufferRW_Particle[_uID].vVelocity.xyz = vVelocity * g_CBuffer_ParticleModule.fSpeed;
					}
                        
                        // To Center
					else if (TRUE == g_CBuffer_ParticleModule.eAddVelocityType)
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
                        //g_SBufferRW_Particle[_uID].vWorldPos.xyz = float3(fRadius * cos(fAngle), fRadius * sin(fAngle), 100.f);
				}
                    
                    
				g_SBufferRW_Particle[_uID].vColor = g_CBuffer_ParticleModule.vSpawnColor;
                                      
				g_SBufferRW_Particle[_uID].fAge = 0.f;
				g_SBufferRW_Particle[_uID].fLifeTime = g_CBuffer_ParticleModule.fMinLifeTime + (g_CBuffer_ParticleModule.fMaxLifeTime - g_CBuffer_ParticleModule.fMinLifeTime) * vOut2.r;
				break;
			}
		}
	}
}


void PModule_Drag(uint _uID)
{
    // 파티클의 현재 속력
	float fSpeed = length(g_SBufferRW_Particle[_uID].vVelocity);
	float fDrag = g_CBuffer_ParticleModule.fStartDrag + (g_CBuffer_ParticleModule.fEndDrag - g_CBuffer_ParticleModule.fStartDrag) * g_SBufferRW_Particle[_uID].fNormalizedAge;
            
	if (fDrag < fSpeed)
	{
		g_SBufferRW_Particle[_uID].vVelocity = normalize(g_SBufferRW_Particle[_uID].vVelocity) * fDrag;
	}
}