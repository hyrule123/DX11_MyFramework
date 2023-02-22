
#include "Shader_STD2d_0_header.hlsli"



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
        vOutColor = float4(1.f, 1.f, 0.f, 1.f);
    }
    
    //애니메이션 사용중일경우
	else if (eANIM2D_FLAG::USEANIM & Data.MTRL_SCALAR_STD2D_FLAG)
    {
		float2 vUV = Data.MTRL_SCALAR_STD2D_ANIM_UV_LEFTTOP + Data.MTRL_SCALAR_STD2D_ANIM_UV_SLICE * _in.vUV;

		vOutColor = g_tex_0.Sample(g_Sampler_0, vUV);
        
  //      //애니메이션의 피벗을 지정(캔버스 사이즈의 LT로부터 스프라이트 이미)
		//float2 vUV = (float2)0.f;
  //      vUV = Data.MTRL_SCALAR_STD2D_ANIM_UV_LEFTTOP + (Data.MTRL_SCALAR_STD2D_PIVOT * _in.vUV);
		//vUV -= ((Data.MTRL_SCALAR_STD2D_PIVOT - Data.MTRL_SCALAR_STD2D_ANIM_UV_SLICE) * 0.5f);
		//vUV -= SHADER_STD2D_vOffset;
    
  //      //
  //      if (
  //      vUV.x > Data.MTRL_SCALAR_STD2D_ANIM_UV_LEFTTOP.x
  //      && vUV.x < Data.MTRL_SCALAR_STD2D_ANIM_UV_LEFTTOP.x + Data.MTRL_SCALAR_STD2D_ANIM_UV_SLICE.x
  //      && vUV.y > Data.MTRL_SCALAR_STD2D_ANIM_UV_LEFTTOP.y
  //      && vUV.y < Data.MTRL_SCALAR_STD2D_ANIM_UV_LEFTTOP.y + Data.MTRL_SCALAR_STD2D_ANIM_UV_SLICE.y
  //      )
  //      {
  //          vOutColor = g_tex_0.Sample(g_Sampler_0, vUV);
  //      }
  //      else
  //      {
		//	discard;
  //          //vOutColor = float4(1.f, 1.f, 0.f, 1.f);
  //      }
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