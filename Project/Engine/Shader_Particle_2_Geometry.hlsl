#include "Shader_Particle_0_header.hlsli"


[maxvertexcount(6)]
void GS_Particle(
	point VS_INOUT _in[1], 
	inout TriangleStream<GS_OUT> _outstream
)
{
    uint id = _in[0].uInstID;
	
	//GS �ܰ迡�� �ش� ��ƼŬ�� ������ ���θ� �Ǵ� �� ���������� ���� ��ƼŬ�̸� return ���ش�.
	if (0 == g_SBuffer_ParticleTransform[id].bActive)
		return;
	
	//������ �� ���������� �̵�
    float3 vParticleViewPos = mul(float4(g_SBuffer_ParticleTransform[id].vWorldPos.xyz, 1.f), g_CBuffer_Transform.matView).xyz;
    
	
    float2 vParticleScale = mul(g_SBuffer_ParticleTransform[id].vWorldScale.xy, 0.5f);
    
	float CosRot = cos(g_SBuffer_ParticleTransform[id].vWorldRotation.z);
	float SinRot = sin(g_SBuffer_ParticleTransform[id].vWorldRotation.z);
	
	row_major float2x2 matRot = { CosRot, -SinRot, SinRot, CosRot };
    
	float2 LT = mul(float2(-vParticleScale.x, vParticleScale.y), matRot);
	float2 RT = mul(vParticleScale, matRot);
	float2 RB = mul(float2(vParticleScale.x, -vParticleScale.y), matRot);
	float2 LB = mul(-vParticleScale, matRot);
    
	
	//���ο� ������ ����(�߾��� point���� ��ġ�� �������� �簢���� �����.
	//�� �������� ������ �簢���̱� ������ ������ �� �簢���� ī�޶� ���ϰ� �ȴ�.
	// 0   1
	//
	// 3   2
	//���� ���� �������� ��ġ�� ����Ѵ�.
    GS_OUT output[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };
    output[0].vSV_Pos = float4(
    vParticleViewPos.xy + LT,
    vParticleViewPos.z, 1.f);
    
    output[1].vSV_Pos = float4(
    vParticleViewPos.xy + RT,
    vParticleViewPos.z, 1.f);
    
    output[2].vSV_Pos = float4(
    vParticleViewPos.xy + RB,
    vParticleViewPos.z, 1.f);
    
    output[3].vSV_Pos = float4(
    vParticleViewPos.xy + LB,
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
	
    
    //���� ���� 4���� �� ������ ����ش�.(�������� : triangleList)
    _outstream.Append(output[0]);
    _outstream.Append(output[1]);
    _outstream.Append(output[2]);
    _outstream.RestartStrip();
    
    _outstream.Append(output[0]);
    _outstream.Append(output[2]);
    _outstream.Append(output[3]);
    _outstream.RestartStrip();

}