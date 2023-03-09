#include "S_0_H_Particle.hlsli"


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
	float3 vParticleViewPos = mul(float4(g_SBuffer_ParticleTransform[id].vWorldPos.xyz, 1.f), g_CBuffer_matCam[g_CBuffer_Mtrl_Scalar.MTRL_SCALAR_INT_CAMIDX].matView).xyz;
    
	//��ƼŬ�� ũ�⸦ ������ Scale�� ����
    float2 vParticleScale = mul(g_SBuffer_ParticleTransform[id].vWorldScale.xy, 0.5f);
    
    
    //ȸ�� ����� ������� ���(ȸ������ 0.f�� �ƴ� ��� ȸ�� ���)
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
       
	
	//���ο� ������ ����(�߾��� point���� ��ġ�� �������� �簢���� �����.
	//�� �������� ������ �簢���̱� ������ ������ �� �簢���� ī�޶� ���ϰ� �ȴ�.
	// 0   1
	//
	// 3   2
	//���� ���� �������� ��ġ�� ����Ѵ�.
	GS_OUT output[4] =
	{
		(GS_OUT[4]) 0.f
	};
	for (int i = 0; i < 4; ++i)
	{
		output[i].vSV_Pos = float4(vParticleViewPos.xy + Vertices[i], vParticleViewPos.z, 1.f);
		output[i].vSV_Pos = mul(output[i].vSV_Pos, g_CBuffer_matCam[g_CBuffer_Mtrl_Scalar.MTRL_SCALAR_INT_CAMIDX].matProj);
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