#include "S_H_SCUnitGround.hlsli"

#include "S_H_Func.hlsli"


float4 PS_BuildingStructure(VS_OUT _in) : SV_TARGET
{
	float4 vOutColor = (float4) 0.f;
    
	tMtrlScalarData Data = (tMtrlScalarData) 0.f;
    
	//인스턴싱을 사용하지 않을 경우->상수버퍼의 데이터를 사용
	if (0 == g_CBuffer_SBufferData.uSBufferCount)
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
    
    //애니메이션 사용중일경우
	else if (eMTRL_SCALAR_STD2D_FLAG::USE_ANIM & Data.MTRL_SCALAR_STD2D_FLAG)
	{
		float2 vUV = _in.vUV;
		if (eMTRL_SCALAR_STD2D_FLAG::NEED_FLIP_X & Data.MTRL_SCALAR_STD2D_FLAG)
			vUV.x = 1.f - vUV.x;
        
        //애니메이션의 Left Top부터 Slice에 자신의 UV값을 곱해서 실제 UV값을 구해준다.
		float2 RealUV = Data.MTRL_SCALAR_STD2D_ANIM_UV_LEFTTOP + Data.MTRL_SCALAR_STD2D_ANIM_UV_SLICE * vUV;

		//텍스처 버퍼 8개 중 현재 그리고자 하는 텍스처 버퍼 번호를 받아와서 샘플링해준다.
		int idx = Data.MTRL_SCALAR_STD2D_ANIM_TEXATLAS_IDX;
		vOutColor = SampleMtrlTex(idx, g_Sampler_0, RealUV);
	}
	else
	{
		vOutColor = g_tex_0.Sample(g_Sampler_0, _in.vUV);
	}
	
	//Alpha Check 
	if (0.f == vOutColor.a)
	{
		discard;
	}
    
	//ColorKey Check
	if (eMTRL_SCALAR_STD2D_FLAG::USE_COLOR_KEY & Data.MTRL_SCALAR_STD2D_FLAG)
	{
		if (all(FLT_EPSILON > abs(vOutColor.rgb - Data.MTRL_SCALAR_STD2D_COLORKEY.rgb)))
			discard;
		
		//if (all(vOutColor.rgb == Data.MTRL_SCALAR_STD2D_COLORKEY.rgb))
		//	discard;
	}
	
	
	if (TRUE == g_CBuffer_Mtrl_Tex.bTexProd)
	{
		//
		if (eMTRL_SCALAR_FLAG_BUILDING_STRUCTURE::ENABLE_PROD_TEX & Data.MTRL_SCALAR_STD2D_FLAG)
		{
			float4 vProdColor = SampleMtrlTex((uint) iTexProdIdx, g_Sampler_0, _in.vUV);
			if (all(FLT_EPSILON < abs(vProdColor.rgb - Data.MTRL_SCALAR_STD2D_COLORKEY.rgb)))
			{
				vOutColor = vProdColor;
			}
		}
	}
	

    
	return vOutColor;
}
