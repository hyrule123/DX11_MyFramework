
#include "Shader_std2d_0_header.hlsli"

VS_OUT VS_std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
	
	//인스턴싱인지 아닌지에 따라서 다른 곳에서 데이터를 받아온다.
	tMtrlScalarData Data = (tMtrlScalarData) 0.f;
	if (0 == g_CBuffer_SBuffer_ShareData[eCBUFFER_SBUFFER_SHAREDATA_IDX::MTRL_SCALAR].uSBufferCount)
	{
		Data = g_CBuffer_Mtrl_Scalar;
	}
	else
	{
		Data = g_SBuffer_Mtrl_Scalar[_in.uInstID];
	}
	
	
	//피벗을 사용하도록 설정했을 경우 정점 위치를 피벗에 맞게 바꿔준다.
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