#include "S_H_Particle.hlsli"

void PModule_Spawn(uint _uID);

static const float M_PI = 3.141592f;
static const float g = 9.81f; // Acceleration due to gravity in m/s^2
static const float rho = 1.225f; // Density of air in kg/m^3
static const float Cd = 0.47f; // Drag coefficient for a sphere
static const float D = 0.001f; // Diameter of water droplet in m
static const float A = 3.141592f * 0.001f * 0.001f / 4; // Cross-sectional area of water droplet in m^2
static const float mu = 1.81e-5; // Dynamic viscosity of air in Pa*s
static const float COEFF_RESTITUTION = 0.8; // coefficient of restitution

[numthreads(128, 1, 1)]
void CS_Particle_RainDrop( uint3 dtID : SV_DispatchThreadID )
{
	
	uint idx = dtID.x;
	
	if ((uint) g_CBuffer_ParticleModule.iMaxParticleCount < idx)
		return;
	
	if (TRUE == g_CBuffer_ParticleModule.bModule_Spawn)
	{
		PModule_Spawn(idx);
	}
	
	
	if (true == g_SBufferRW_ParticleTransform[idx].bActive)
	{
		g_SBufferRW_ParticleTransform[idx].fAge += g_CBuffer_GlobalData.fDeltaTime;
		
		if (g_SBufferRW_ParticleTransform[idx].fAge > g_SBufferRW_ParticleTransform[idx].fLifeTime)
		{
			g_SBufferRW_ParticleTransform[idx].bActive = FALSE;
			return;
		}
		
		

		g_SBufferRW_ParticleTransform[idx].vVelocity.y -= g_CBuffer_ParticleModule.fGravity * g_CBuffer_GlobalData.fDeltaTime * 100.f;
		
	
		g_SBufferRW_ParticleTransform[idx].vWorldPos.xy += g_SBufferRW_ParticleTransform[idx].vVelocity.xy * g_CBuffer_GlobalData.fDeltaTime;
		
		
		float4 NDCPos = mul(g_SBufferRW_ParticleTransform[idx].vWorldPos, g_CBuffer_matCam[g_CBuffer_Mtrl_Scalar.MTRL_SCALAR_INT_CAMIDX].matVP);
		if (NDCPos.y < -1.f)
		{
			g_SBufferRW_ParticleTransform[idx].vVelocity.y *= -1.f * 0.8f;
			
			g_SBufferRW_ParticleTransform[idx].vWorldPos.y += g_SBufferRW_ParticleTransform[idx].vVelocity.y * g_CBuffer_GlobalData.fDeltaTime;
		}
		
		if(NDCPos.x < -1.f || NDCPos.x > 1.f)
		{
			g_SBufferRW_ParticleTransform[idx].vVelocity.x *= -1.f;
			
			g_SBufferRW_ParticleTransform[idx].vWorldPos.x += g_SBufferRW_ParticleTransform[idx].vVelocity.x * g_CBuffer_GlobalData.fDeltaTime;
		}
		
		g_SBufferRW_ParticleTransform[idx].vWorldRotation += g_SBufferRW_ParticleTransform[idx].vRotPerSec;
		
	}
	
}

void PModule_Spawn(uint _uID)
{
    // 파티클이 비활성화 상태인 경우 도로 살려내는 작업 시작
	if (FALSE == g_SBufferRW_ParticleTransform[_uID].bActive)
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
				g_SBufferRW_ParticleTransform[_uID].bActive = 1;
                    
                // 랜덤 결과를 받을 변수
				const static float umax = float(0xffffffff);
				float fRand1 = float(XorShift64Plus(g_SBufferRW_Particle_Shared[0].uSeeds)) / umax;
				float fRand2 = float(XorShift64Plus(g_SBufferRW_Particle_Shared[0].uSeeds)) / umax;
				float fRand3 = float(XorShift64Plus(g_SBufferRW_Particle_Shared[0].uSeeds)) / umax;
                
                    
                // Box 스폰
				if (g_CBuffer_ParticleModule.eSpawnShapeType == 0)
				{
					g_SBufferRW_ParticleTransform[_uID].vLocalPos.xyz = float3(g_CBuffer_ParticleModule.vBoxShapeScale.x * fRand1 - g_CBuffer_ParticleModule.vBoxShapeScale.x * 0.5f
                                                      , g_CBuffer_ParticleModule.vBoxShapeScale.y * fRand2 - g_CBuffer_ParticleModule.vBoxShapeScale.y * 0.5f
                                                      , g_CBuffer_ParticleModule.vBoxShapeScale.z * fRand3 - g_CBuffer_ParticleModule.vBoxShapeScale.z * 0.5f);
					g_SBufferRW_ParticleTransform[_uID].vWorldPos.xyz = g_SBufferRW_ParticleTransform[_uID].vLocalPos.xyz + g_CBuffer_Mtrl_Scalar.MTRL_SCALAR_OWNER_OBJ_POS.xyz;
                        
                        
                        // 스폰 크기 범위내에서 랜덤 크기로 지정 (Min, Max 가 일치하면 고정크기)
					float4 vSpawnScale = g_CBuffer_ParticleModule.vSpawnScaleMin + (g_CBuffer_ParticleModule.vSpawnScaleMax - g_CBuffer_ParticleModule.vSpawnScaleMin) * fRand3;
					g_SBufferRW_ParticleTransform[_uID].vWorldScale.xyz = vSpawnScale.xyz;
					
					
					
					g_SBufferRW_ParticleTransform[_uID].fMass = 0.001f;
				}
				
						

                   
                // bModule_AddVelocity 모듈
				if (TRUE == g_CBuffer_ParticleModule.bModule_AddVelocity)
				{
					//g_SBufferRW_ParticleTransform[_uID].vVelocity = normalize
					g_SBufferRW_ParticleTransform[_uID].vVelocity = float4(300.f, -1.f, 0.f, 0.f);
					
					float rnd = (0.5f - fRand2) * 2.f;
					
					g_SBufferRW_ParticleTransform[_uID].vVelocity.x *= rnd;

					g_SBufferRW_ParticleTransform[_uID].vRotPerSec.z = rnd * 0.5f;
				}
				

                    
                // Sphere 스폰
				else if (g_CBuffer_ParticleModule.eSpawnShapeType == 1)
				{
					float fRadius = 500.f; //vOut1.r * 200.f;
					float fAngle = fRand2 * 6.2831852f;
                        //g_SBufferRW_ParticleTransform[_uID].vWorldPos.xyz = float3(fRadius * cos(fAngle), fRadius * sin(fAngle), 100.f);
				}
                    
                    
				g_SBufferRW_ParticleTransform[_uID].vColor = g_CBuffer_ParticleModule.vSpawnColor;
                                      
				g_SBufferRW_ParticleTransform[_uID].fAge = 0.f;
				g_SBufferRW_ParticleTransform[_uID].fLifeTime = g_CBuffer_ParticleModule.fMinLifeTime + (g_CBuffer_ParticleModule.fMaxLifeTime - g_CBuffer_ParticleModule.fMinLifeTime) * fRand2;
				break;
			}
		}
	}
}