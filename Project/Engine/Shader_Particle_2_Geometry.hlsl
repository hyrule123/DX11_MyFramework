#include "Shader_Particle_0_header.hlsli"


[maxvertexcount(6)]
void GS_Particle(
	point VS_INOUT _in[1], 
	inout TriangleStream<GS_OUT> _outstream
)
{
    uint id = _in[0].uInstID;
	
	//GS 단계에서 해당 파티클의 렌더링 여부를 판단 후 렌더링되지 않을 파티클이면 return 해준다.
	if (0 == g_SBuffer_ParticleTransform[id].bActive)
		return;
	
	//정점을 뷰 공간상으로 이동
    float3 vParticleViewPos = mul(float4(g_SBuffer_ParticleTransform[id].vWorldPos.xyz, 1.f), g_CBuffer_matCam.matView).xyz;
    
	//파티클의 크기를 지정한 Scale로 조정
    float2 vParticleScale = mul(g_SBuffer_ParticleTransform[id].vWorldScale.xy, 0.5f);
    
    
    //회전 모듈을 사용중일 경우(회전값이 0.f가 아닐 경우 회전 계산)
	float2 Vertices[4] = { 
        float2(-vParticleScale.x,  vParticleScale.y), 
        float2( vParticleScale.x,  vParticleScale.y),
        float2( vParticleScale.x, -vParticleScale.y), 
        float2(-vParticleScale.x, -vParticleScale.y) 
        };
	if (!(all(float3(0.f, 0.f, 0.f) == g_SBuffer_ParticleTransform[id].vWorldRotation.xyz)))
	{
		float CosRot = cos(g_SBuffer_ParticleTransform[id].vWorldRotation.z);
		float SinRot = sin(g_SBuffer_ParticleTransform[id].vWorldRotation.z);
	
		row_major float2x2 matRot = { CosRot, -SinRot, SinRot, CosRot };
    
		for (int i = 0; i < 4; ++i)
		{
			Vertices[i] = mul(Vertices[i], matRot);
		}
	}
       
	
	//새로운 정점을 생성(중앙인 point정점 위치를 기준으로 사각형을 만든다.
	//뷰 공간에서 생성한 사각형이기 떄문에 무조건 이 사각형은 카메라를 향하게 된다.
	// 0   1
	//
	// 3   2
	//새로 만든 정점으로 위치를 계산한다.
	GS_OUT output[4] =
	{
		(GS_OUT[4]) 0.f
	};
	for (int i = 0; i < 4; ++i)
	{
		output[i].vSV_Pos = float4(vParticleViewPos.xy + Vertices[i], vParticleViewPos.z, 1.f);
		output[i].vSV_Pos = mul(output[i].vSV_Pos, g_CBuffer_matCam.matProj);
		output[i].uInstID = _in[0].uInstID;
	}

    output[0].vUV = float2(0.f, 0.f);
    output[1].vUV = float2(1.f, 0.f);
    output[2].vUV = float2(1.f, 1.f);
    output[3].vUV = float2(0.f, 1.f);
	
    
    //만든 정점 4개로 새 정점을 찍어준다.(토폴로지 : triangleList)
    _outstream.Append(output[0]);
    _outstream.Append(output[1]);
    _outstream.Append(output[2]);
    _outstream.RestartStrip();
    
    _outstream.Append(output[0]);
    _outstream.Append(output[2]);
    _outstream.Append(output[3]);
    _outstream.RestartStrip();

}