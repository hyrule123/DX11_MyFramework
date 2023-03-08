#include "S_0_H_STD2DLight.hlsli"

VS_OUT VS_std2D_Light(VS_IN _in)
{
	VS_OUT output = (VS_OUT) 0.f;
	
	//�ν��Ͻ����� �ƴ����� ���� �ٸ� ������ �����͸� �޾ƿ´�.
	tMtrlScalarData Data = GetMtrlScalarData(_in.uInstID);
	
	
	//�ǹ��� ����ϵ��� �������� ��� ���� ��ġ�� �ǹ��� �°� �ٲ��ش�.
	if (eMTRL_SCALAR_STD2D_FLAG::USE_PIVOT & Data.MTRL_SCALAR_STD2D_FLAG)
	{
		float2 PivotOffset = float2(0.5f, 0.5f) - Data.MTRL_SCALAR_STD2D_PIVOT;
		_in.vLocalPos.xy += PivotOffset;
	}
	
	//���� ������ �� ��꿡 ���� ��ġ�� �ʿ��ϹǷ� WVP ����� �ʿ����� ����...
	output.vWorldPos = mul(float4(_in.vLocalPos, 1.f), Data.MTRL_SCALAR_MAT_WORLD).xyz;
	output.vPosSV = mul(float4(output.vWorldPos, 1.f), g_CBuffer_matCam[Data.MTRL_SCALAR_INT_CAMIDX].matVP);
	
	output.vUV = _in.vUV;
	output.uInstID = _in.uInstID;
	
	return output;
}