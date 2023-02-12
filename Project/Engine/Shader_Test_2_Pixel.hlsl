
#include "Shader_test_0_header.hlsli"




// pixel shader
float4 PS_test(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
  
    if (g_CBuffer_MtrlData.INT_0 == 0)
        vColor = g_tex_0.Sample(g_Sampler_0, _in.vOutUV);
    else if (g_CBuffer_MtrlData.INT_0 == 1)
        vColor = g_tex_0.Sample(g_Sampler_1, _in.vOutUV);
    
    if (all(vColor.rgb == COLOR_KEY.rgb))
        discard;
    
    vColor.a = 0.5f;
    
    return vColor;
}
