
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
    
    
    //�����ؽ�ó�� �������� �������� ������ ����Ÿ ������ return;
    if (FALSE == g_CBuffer_Mtrl_Tex.bTEX_0)
    {
        vOutColor = float4(1.f, 1.f, 0.f, 1.f);
    }
    
    //�ִϸ��̼� ������ϰ��
	else if (eANIM2D_FLAG::USEANIM & Data.MTRL_SCALAR_STD2D_FLAG)
    {
        //�ִϸ��̼��� Left Top���� Slice�� �ڽ��� UV���� ���ؼ� ���� UV���� �����ش�.
		float2 vUV = Data.MTRL_SCALAR_STD2D_ANIM_UV_LEFTTOP + Data.MTRL_SCALAR_STD2D_ANIM_UV_SLICE * _in.vUV;

		vOutColor = g_tex_0.Sample(g_Sampler_0, vUV);
       
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