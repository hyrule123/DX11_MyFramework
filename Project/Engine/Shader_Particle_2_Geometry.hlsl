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
    float3 vParticleViewPos = mul(float4(g_SBuffer_ParticleTransform[id].vWorldPos.xyz, 1.f), g_CBuffer_Transform.matView).xyz;
    
	
    float2 vParticleScale = mul(g_SBuffer_ParticleTransform[id].vWorldScale.xy, 0.5f);
    
	float CosRot = cos(g_SBuffer_ParticleTransform[id].vWorldRotation.z);
	float SinRot = sin(g_SBuffer_ParticleTransform[id].vWorldRotation.z);
	
	float CosX = CosRot * vParticleScale.x;
	float CosY = CosRot * vParticleScale.y;
	float SinX = SinRot * vParticleScale.x;
	float SinY = SinRot * vParticleScale.y;
    
	float2 vtx0 = float2(-CosX - SinY, -SinX + CosY);
	float2 vtx1 = float2(CosX + SinY, -SinX + CosY);
	float2 vtx2 = float2(CosX + SinY, +SinX - CosY);
	float2 vtx3 = float2(-CosX - SinY, +SinX - CosY);
  
	
	//새로운 정점을 생성(중앙인 point정점 위치를 기준으로 사각형을 만든다.
	//뷰 공간에서 생성한 사각형이기 떄문에 무조건 이 사각형은 카메라를 향하게 된다.
	// 0   1
	//
	// 3   2
	//새로 만든 정점으로 위치를 계산한다.
    GS_OUT output[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };
    output[0].vSV_Pos = float4(
    vParticleViewPos.x + vtx0.x,
    vParticleViewPos.y + vtx0.y,
    //(vParticleViewPos.x - vParticleScale.x),
    //(vParticleViewPos.y + vParticleScale.y),
    vParticleViewPos.z, 1.f);
    
    output[1].vSV_Pos = float4(
    vParticleViewPos.x + vtx1.x,
    vParticleViewPos.y + vtx1.y, 
    vParticleViewPos.z, 1.f);
    
    output[2].vSV_Pos = float4(
    vParticleViewPos.x + vtx2.x,
    vParticleViewPos.y + vtx2.y, 
    vParticleViewPos.z, 1.f);
    
    output[3].vSV_Pos = float4(
    vParticleViewPos.x +vtx3.x,
    vParticleViewPos.y +vtx3.y, 
    vParticleViewPos.z, 1.f);
    
    
    
    for (int i = 0; i < 4; ++i)
    {
        output[i].vSV_Pos = mul(output[i].vSV_Pos, g_CBuffer_Transform.matProj);
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