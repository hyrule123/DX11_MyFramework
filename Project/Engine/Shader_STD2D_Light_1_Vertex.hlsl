#include "Shader_std2D_Light_0_header.hlsli"

VS_OUT VS_std2D_Light(VS_IN _in)
{
	VS_OUT output = (VS_OUT) 0.f;
	
	//�ν��Ͻ����� �ƴ����� ���� �ٸ� ������ �����͸� �޾ƿ´�.
	tMtrlScalarData Data = GetMtrlScalarData(_in.uInstID);
	
	
	//�ǹ��� ����ϵ��� �������� ��� ���� ��ġ�� �ǹ��� �°� �ٲ��ش�.
	if (eANIM2D_FLAG::USEPIVOT & Data.MTRL_SCALAR_STD2D_FLAG)
	{
		float2 PivotOffset = float2(0.5f, 0.5f) - Data.MTRL_SCALAR_STD2D_PIVOT;
		_in.vLocalPos.xy += PivotOffset;
	}
	
	
	output.vWorldPos = mul(float4(_in.vLocalPos, 1.f), Data.MTRL_SCALAR_MAT_WORLD);
	output.vPosSV = mul(float4(output.vWorldPos, 1.f), g_CBuffer_matCam[Data.MTRL_SCALAR_INT_CAMIDX].matVP);
	
	output.vUV = _in.vUV;
	output.uInstID = _in.uInstID;
	
	return output;
}