
#include "Shader_STD2d_0_header.hlsli"



float4 PS_std2D(VS_OUT _in) : SV_TARGET
{
    float4 vOutColor = (float4) 0.f;
    
    //�����ؽ�ó�� �������� �������� ������ ����Ÿ ������ return;
    if (FALSE == g_CBuffer_Mtrl_Tex.bTEX_0)
    {
        vOutColor = float4(1.f, 1.f, 0.f, 1.f);
    }
    
    //�ִϸ��̼� ������ϰ��
    else if (0 != SHADER_STD2D_bAnimUse)
    {
            //�ִϸ��̼��� �ǹ��� ����(ĵ���� �������� LT�κ��� ��������Ʈ �̹�)
		float2 vUV = SHADER_STD2D_vLT + (SHADER_STD2D_vCanvasSize * _in.vUV);
		vUV -= ((SHADER_STD2D_vCanvasSize - SHADER_STD2D_vSlice) * 0.5f);
		vUV -= SHADER_STD2D_vOffset;
    
        //
        if (
        vUV.x > SHADER_STD2D_vLT.x
        && vUV.x < SHADER_STD2D_vLT.x + SHADER_STD2D_vSlice.x
        && vUV.y > SHADER_STD2D_vLT.y
        && vUV.y < SHADER_STD2D_vLT.y + SHADER_STD2D_vSlice.y
        )
        {
            vOutColor = g_tex_0.Sample(g_Sampler_0, vUV);
        }
        else
        {
            vOutColor = float4(1.f, 1.f, 0.f, 1.f);
        }
    }
    else
    {
        vOutColor = g_tex_0.Sample(g_Sampler_0, _in.vUV);
    }
	
    //Alpha Check + Color Key Check
    if (0.f == vOutColor.a || all(vOutColor.rgb == COLOR_KEY.rgb))
    {
        discard;
    }
    
    return vOutColor;
}