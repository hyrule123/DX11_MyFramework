
#include "Shader_std2d_0_header.hlsli"



float4 PS_std2D(VS_OUT _in) : SV_TARGET
{
    float4 vOutColor = (float4) 0.f;
	
	if(0 != g_btex_0)
    {
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }
    else
    {
        vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    }
	
    //Alpha Check + Color Key Check
    if (0.f == vOutColor.a || all(vOutColor.rgb == COLOR_KEY.rgb))
    {
        discard;
    }
    
    return vOutColor;
}