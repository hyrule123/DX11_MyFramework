#include "S_H_SCUnitGround.hlsli"

#include "S_H_Func.hlsli"


float4 PS_Mineral(VS_OUT _in) : SV_TARGET
{
	float4 vOutColor = (float4) 0.f;
    
	tMtrlScalarData Data = (tMtrlScalarData) 0.f;
    
	//인스턴싱을 사용하지 않을 경우->상수버퍼의 데이터를 사용
	if (0 == g_CBuffer_SBufferData[eCBUFFER_SBUFFER_SHAREDATA_IDX::MTRL_SCALAR].uSBufferCount)
	{
		Data = g_CBuffer_Mtrl_Scalar;
	}
	
	//인스턴싱을 사용할 경우 -> 구조화 버퍼의 데이터를 사용
	else
	{
		Data = g_SBuffer_Mtrl_Scalar[_in.uInstID];
	}
    
    //메인텍스처가 존재하지 않을경우는 무조건 마젠타 색상을 return;
	if (FALSE == g_CBuffer_Mtrl_Tex.bTEX_0)
	{
		vOutColor = float4(1.f, 0.f, 1.f, 1.f);
	}
	
	
	_in.vUV.x = (0.25f * _in.vUV.x) + 0.25f * (float) Data.MTRL_SCALAR_MINERAL_WEAR_LEVEL;
	
	vOutColor = SampleMtrlTex((uint)Data.MTRL_SCALAR_MINERAL_TEXINDEX, g_Sampler_0, _in.vUV);
	
	//Alpha Check 
	if (0.f == vOutColor.a)
	{
		discard;
	}
    
	//ColorKey Check
	else if (eMTRL_SCALAR_STD2D_FLAG::USE_COLOR_KEY & Data.MTRL_SCALAR_STD2D_FLAG)
	{
		if (all(FLT_EPSILON > abs(vOutColor.rgb - Data.MTRL_SCALAR_STD2D_COLORKEY.rgb)))
			discard;
	}
	
	
	return vOutColor;
}
