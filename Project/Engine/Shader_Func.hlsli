#ifndef SHADER_FUNC
#define SHADER_FUNC

#include "Shader_header_register.hlsli"


inline tMtrlScalarData GetMtrlScalarData(uint _uInstanceID)
{
	tMtrlScalarData Data = (tMtrlScalarData) 0.f;
    
	if (0 != g_CBuffer_SBuffer_ShareData[eCBUFFER_SBUFFER_SHAREDATA_IDX::MTRL_SCALAR].uSBufferCount)
	{
		Data = g_SBuffer_Mtrl_Scalar[_uInstanceID];
	}
	else
	{
		Data = g_CBuffer_Mtrl_Scalar;
	}
    
	return Data;
}


inline float4 SampleMtrlTex(uint _uTexRegisterIdx, SamplerState _Sampler, float2 _vUV)
{
	float4 SampleColor = (float4) 0.f;
    
	switch (_uTexRegisterIdx)
	{
		case eMTRLDATA_PARAM_TEX::_0:
			SampleColor = g_tex_0.Sample(_Sampler, _vUV);
			break;

		case eMTRLDATA_PARAM_TEX::_1:
			SampleColor = g_tex_1.Sample(_Sampler, _vUV);
			break;

		case eMTRLDATA_PARAM_TEX::_2:
			SampleColor = g_tex_2.Sample(_Sampler, _vUV);
			break;
            
		case eMTRLDATA_PARAM_TEX::_3:
			SampleColor = g_tex_3.Sample(_Sampler, _vUV);
			break;
            
		case eMTRLDATA_PARAM_TEX::_4:
			SampleColor = g_tex_4.Sample(_Sampler, _vUV);
			break;
            
		case eMTRLDATA_PARAM_TEX::_5:
			SampleColor = g_tex_5.Sample(_Sampler, _vUV);
			break;
            
		case eMTRLDATA_PARAM_TEX::_6:
			SampleColor = g_tex_6.Sample(_Sampler, _vUV);
			break;
            
		case eMTRLDATA_PARAM_TEX::_7:
			SampleColor = g_tex_7.Sample(_Sampler, _vUV);
			break;
	};

	return SampleColor;
}




uint XorShift64Plus(inout uint2 state)
{
	uint x = state.x;
	uint y = state.y;
	state.x = y;
	x ^= x << 23;
	state.y = x ^ y ^ (x >> 17) ^ (y >> 26);
	return state.y + y;
}

static float GaussianFilter[5][5] =
{
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.0219f, 0.0983f, 0.1621f, 0.0983f, 0.0219f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
};

#define PI 3.14159265f
#define PI_DOUBLE 6.2831853f

void GaussianSample(in Texture2D _Tex_Noise, float2 _vResolution, float _NormalizedThreadID, out float3 _vOut)
{
    //���� ����ȭ�� ������ ID�� UV������ ����Ѵ�.
    float2 vUV = float2(_NormalizedThreadID, 0.5f);
    
    //x�� �ణ�� �������� �ֱ� ���� ���������� �������ش�.
    vUV.x += g_CBuffer_GlobalData.fAccTime * 0.5f;
    
    // sin �׷����� �ؽ����� ���ø� ��ġ UV �� ���
    //fAccTime : ���� ���� �� ������ ���� ���� �ð�. �� ���� Ȱ���ϸ� ���α׷����� ���������� �������� �̵���ų �� �ִ�.
    //
    vUV.y -= (sin((_NormalizedThreadID - (g_CBuffer_GlobalData.fAccTime /*�׷��� ���� �̵� �ӵ�*/)) * PI_DOUBLE * 10.f /*�ݺ��ֱ�*/) * 0.5f);
    
    //UV���� 0 ~ 1 ������ ���� ����� ��� ���̷� �������ش�. ���������� �ƴϿ��� �ణ�� �ұ�Ģ���� ���� ���糭���� �������.
    if (1.f < vUV.x)
        vUV.x = frac(vUV.x);
    else if (vUV.x < 0.f)
        vUV.x = 1.f + frac(vUV.x);
    
    if (1.f < vUV.y)
        vUV.y = frac(vUV.y);
    else if (vUV.y < 0.f)
        vUV.y = 1.f + frac(vUV.y);
    
    //������� UV�� �ؽ�ó�� ����� ���� �ָ� ������ �ȼ��� �ε��� ��ȣ�� ���� �� �ִ�.
    int2 pixel = vUV * _vResolution;
    
    //5 * 5¥�� ����þ� ���͸� ����(2, 2)���� �����ϱ� ���ؼ� ����� �ȼ����� 3ĭ�� �о��ش�.
    //
    // 0 1 2 3 4    
    // 1
    // 2  (2, 2) = ����
    // 3
    // 4
    const static int2 offset = int2(-2, -2);
    float3 vOut = (float3) 0.f;
    
    //��ȸ�� �����ָ鼭 �ش� �ε����� ���� ����þ� ���� ���� ���ؼ� ���� ���� �ջ��Ѵ�.
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            vOut += (_Tex_Noise[pixel + offset + int2(j, i)] * GaussianFilter[i][j]).rgb;
        }
    }
    
    _vOut = vOut;
}


#endif