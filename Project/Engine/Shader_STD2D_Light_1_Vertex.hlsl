#include "Shader_std2D_Light_0_header.hlsli"

VS_OUT VS_std2D_Light(VS_IN _in)
{
	VS_OUT output = (VS_OUT) 0.f;
	
	//인스턴싱인지 아닌지에 따라서 다른 곳에서 데이터를 받아온다.
	tMtrlScalarData Data = GetMtrlScalarData(_in.uInstID);
	
	
	//피벗을 사용하도록 설정했을 경우 정점 위치를 피벗에 맞게 바꿔준다.
	if (eMTRL_SCALAR_STD2D_FLAG::USEPIVOT & Data.MTRL_SCALAR_STD2D_FLAG)
	{
		float2 PivotOffset = float2(0.5f, 0.5f) - Data.MTRL_SCALAR_STD2D_PIVOT;
		_in.vLocalPos.xy += PivotOffset;
	}
	
	//여긴 어차피 빛 계산에 월드 위치가 필요하므로 WVP 행렬이 필요하지 않음...
	output.vWorldPos = mul(float4(_in.vLocalPos, 1.f), Data.MTRL_SCALAR_MAT_WORLD).xyz;
	output.vPosSV = mul(float4(output.vWorldPos, 1.f), g_CBuffer_matCam[Data.MTRL_SCALAR_INT_CAMIDX].matVP);
	
	output.vUV = _in.vUV;
	output.uInstID = _in.uInstID;
	
	return output;
}