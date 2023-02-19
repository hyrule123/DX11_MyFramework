
#include "Shader_test_0_header.hlsli"

// vertex shader
// LocalSpace ��ü�� NDC ��ǥ��� �̵�
VS_OUT VS_test(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
	
    //�ν��Ͻ����� �׸� ������ 0���� �����Ǿ� ������ �ν��Ͻ��� �ƴ�.
	if (0 == g_CBuffer_SBuffer_ShareData[eCBUFFER_SBUFFER_SHAREDATA_IDX::MTRL_SCALAR].uSBufferCount)
	{
		output.vPosition = mul(float4(_in.vPos, 1.f), g_CBuffer_Mtrl_Scalar.MTRL_SCALAR_MAT_WVP);
	}
	else
	{
		output.vPosition = mul(float4(_in.vPos, 1.f), g_SBuffer_Mtrl_Scalar[_in.uInstID].MTRL_SCALAR_MAT_WVP);
	}
    
    // �Է����� ���� ������ǥ�� ������� ���� ���ؼ� ���
	
    //output.vOutColor = _in.vColor;
    output.vOutUV = _in.vUV;
	output.uInstID = _in.uInstID;
    
    return output;
}