#include "Shader_Debug_0_Header.hlsli"

VS_OUT VS_Debug( VS_IN _in )
{
	tMtrlScalarData Data = GetMtrlScalarData(_in.uInstID);
	
    VS_OUT _out = (VS_OUT) 0.f;
	
	_out.vPos = mul(float4(_in.vPos, 1.f), Data.MTRL_SCALAR_DEBUG_MAT_WORLD);
	_out.vPos = mul(float4(_out.vPos), g_CBuffer_matCam[g_CBuffer_Mtrl_Scalar.MTRL_SCALAR_INT_CAMIDX].matVP);
	
	_out.uInstID = _in.uInstID;
	
	return _out;
}