
#include "Shader_STD2d_0_header.hlsli"

#include "Shader_Func.hlsli"


float4 PS_std2D(VS_OUT _in) : SV_TARGET
{
    float4 vOutColor = (float4) 0.f;
    
	tMtrlScalarData Data = (tMtrlScalarData) 0.f;
    
	if (0 == g_CBuffer_SBuffer_ShareData[eCBUFFER_SBUFFER_SHAREDATA_IDX::MTRL_SCALAR].uSBufferCount)
	{
		Data = g_CBuffer_Mtrl_Scalar;
	}
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
    else if (eANIM2D_FLAG::USEANIM & Data.MTRL_SCALAR_STD2D_FLAG)
    {
        //애니메이션의 Left Top부터 Slice에 자신의 UV값을 곱해서 실제 UV값을 구해준다.
		float2 vUV = Data.MTRL_SCALAR_STD2D_ANIM_UV_LEFTTOP + Data.MTRL_SCALAR_STD2D_ANIM_UV_SLICE * _in.vUV;

		int idx = Data.MTRL_SCALAR_STD2D_ANIM_TEXATLAS_IDX;
		vOutColor = SampleMtrlTex(idx, g_Sampler_0, vUV);
        

	}
    else
    {
        vOutColor = g_tex_0.Sample(g_Sampler_0, _in.vUV);
    }
    
   
	
    //Alpha Check + Color Key Check
	if (0.f == vOutColor.a || all(vOutColor.rgb == Data.MTRL_SCALAR_STD2D_COLORKEY.rgb))
    {
        discard;
    }
    
    return vOutColor;
}