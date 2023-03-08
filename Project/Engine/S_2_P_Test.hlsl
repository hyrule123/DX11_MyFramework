
#include "S_0_H_Test.hlsli"




// pixel shader
float4 PS_test(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
	if (0 == g_CBuffer_SBuffer_ShareData[eCBUFFER_SBUFFER_SHAREDATA_IDX::MTRL_SCALAR].uSBufferCount)
	{
		if (g_CBuffer_Mtrl_Scalar.INT_0 == 0)
			vColor = g_tex_0.Sample(g_Sampler_0, _in.vOutUV);
		else if (g_CBuffer_Mtrl_Scalar.INT_0 == 1)
			vColor = g_tex_0.Sample(g_Sampler_1, _in.vOutUV);
    
		vColor.a = 0.5f;
	}
	else
	{
		if (g_SBuffer_Mtrl_Scalar[_in.uInstID].INT_0 == 0)
			vColor = g_tex_0.Sample(g_Sampler_0, _in.vOutUV);
		else if (g_SBuffer_Mtrl_Scalar[_in.uInstID].INT_0 == 1)
			vColor = g_tex_0.Sample(g_Sampler_1, _in.vOutUV);
	}
  
	return vColor;
}
