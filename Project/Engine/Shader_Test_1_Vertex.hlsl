
#include "Shader_test_0_header.hlsli"

// vertex shader
// LocalSpace ��ü�� NDC ��ǥ��� �̵�
VS_OUT VS_test(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
	
	tMtrlScalarData Data = GetMtrlScalarData(_in.uInstID);
	
	if(eMTRL_SCALAR_STD2D_FLAG::USEWVP & Data.MTRL_SCALAR_STD2D_FLAG)
	{
		output.vPosition = mul(float4(_in.vPos, 1.f), Data.MTRL_SCALAR_MAT_WVP);
	}
	else
	{
		output.vPosition = mul(float4(_in.vPos, 1.f), Data.MTRL_SCALAR_MAT_WORLD);
		output.vPosition = mul(output.vPosition, g_CBuffer_matCam[Data.MTRL_SCALAR_INT_CAMIDX].matVP);
	}
	
    
    // �Է����� ���� ������ǥ�� ������� ���� ���ؼ� ���
	
    //output.vOutColor = _in.vColor;
    output.vOutUV = _in.vUV;
	output.uInstID = _in.uInstID;
    
    return output;
}