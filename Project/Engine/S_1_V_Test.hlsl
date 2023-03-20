#include "S_0_H_Test.hlsli"

// vertex shader
// LocalSpace 물체를 NDC 좌표계로 이동
VS_OUT VS_Test(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
	
	tMtrlScalarData Data = GetMtrlScalarData(_in.uInstID);
	
	if(eMTRL_SCALAR_STD2D_FLAG::USE_VP & Data.MTRL_SCALAR_STD2D_FLAG)
	{
		output.vPosition = mul(float4(_in.vPos, 1.f), Data.MTRL_SCALAR_MAT_WORLD);
		output.vPosition = mul(output.vPosition, g_CBuffer_matCam[Data.MTRL_SCALAR_INT_CAMIDX].matVP);
	}
	else
	{
		output.vPosition = mul(float4(_in.vPos, 1.f), Data.MTRL_SCALAR_MAT_WVP);
	}
	
    // 입력으로 들어온 정점좌표에 상수버퍼 값을 더해서 출력
	
    //output.vOutColor = _in.vColor;
    output.vOutUV = _in.vUV;
	output.uInstID = _in.uInstID;
    
    return output;
}
