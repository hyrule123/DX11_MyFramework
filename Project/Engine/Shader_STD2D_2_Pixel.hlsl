
#include "Shader_STD2d_0_header.hlsli"



float4 PS_std2D(VS_OUT _in) : SV_TARGET
{
    float4 vOutColor = (float4) 0.f;
    
    //�����ؽ�ó�� �������� �������� ������ ����Ÿ ������ return;
    if (0 == g_CBuffer_MtrlData.bTEX_0)
    {
        vOutColor = float4(1.f, 1.f, 0.f, 1.f);
    }
    
    //�ִϸ��̼� ������ϰ��
    else if (0 != bAnimUse)
    {
            //�ִϸ��̼��� �ǹ��� ����(ĵ���� �������� LT�κ��� ��������Ʈ �̹�)
        float2 vUV = LeftTop + (CanvasSize * _in.vUV);
        vUV -= ((CanvasSize - Slice) * 0.5f);
        vUV -= Offset;
    
        //
        if (
        vUV.x > LeftTop.x
        && vUV.x < LeftTop.x + Slice.x
        && vUV.y > LeftTop.y
        && vUV.y < LeftTop.y + Slice.y
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