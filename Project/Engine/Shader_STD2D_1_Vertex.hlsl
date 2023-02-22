
#include "Shader_std2d_0_header.hlsli"

VS_OUT VS_std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
	
	//�ν��Ͻ����� �ƴ����� ���� �ٸ� ������ �����͸� �޾ƿ´�.
	tMtrlScalarData Data = (tMtrlScalarData) 0.f;
	if (0 == g_CBuffer_SBuffer_ShareData[eCBUFFER_SBUFFER_SHAREDATA_IDX::MTRL_SCALAR].uSBufferCount)
	{
		Data = g_CBuffer_Mtrl_Scalar;
	}
	else
	{
		Data = g_SBuffer_Mtrl_Scalar[_in.uInstID];
	}
	
	
	//�ǹ��� ����ϵ��� �������� ��� ���� ��ġ�� �ǹ��� �°� �ٲ��ش�.
	if (eANIM2D_FLAG::USEPIVOT & Data.MTRL_SCALAR_STD2D_FLAG)
	{
		float2 PivotOffset = float2(0.5f, 0.5f) - Data.MTRL_SCALAR_STD2D_PIVOT;
		_in.vPos.xy += PivotOffset;
	}
	
	
	output.vPosSV = mul(float4(_in.vPos, 1.f), Data.MTRL_SCALAR_MAT_WORLD);
	output.vPosSV = mul(output.vPosSV, g_CBuffer_matCam.matVP);
	
    output.vUV = _in.vUV;
	output.uInstID = _in.uInstID;
	
	return output;
}