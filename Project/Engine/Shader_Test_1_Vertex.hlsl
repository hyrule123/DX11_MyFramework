
#include "Shader_test_0_header.hlsli"

// vertex shader
// LocalSpace 물체를 NDC 좌표계로 이동
VS_OUT VS_test(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
	
    //인스턴싱으로 그릴 갯수가 0개로 설정되어 있으면 인스턴싱이 아님.
	if (0 == g_CBuffer_SBuffer_ShareData[eCBUFFER_SBUFFER_SHAREDATA_IDX::MTRL_SCALAR].uSBufferCount)
	{
		output.vPosition = mul(float4(_in.vPos, 1.f), g_CBuffer_Mtrl_Scalar.MTRL_SCALAR_MAT_WORLD);
	}
	else
	{
		output.vPosition = mul(float4(_in.vPos, 1.f), g_SBuffer_Mtrl_Scalar[_in.uInstID].MTRL_SCALAR_MAT_WORLD);
	}
	
	output.vPosition = mul(output.vPosition, g_CBuffer_matCam.matVP);
    
    // 입력으로 들어온 정점좌표에 상수버퍼 값을 더해서 출력
	
    //output.vOutColor = _in.vColor;
    output.vOutUV = _in.vUV;
	output.uInstID = _in.uInstID;
    
    return output;
}