#include "S_0_H_Debug.hlsli"

VS_OUT VS_Debug( VS_IN _in )
{
	tMtrlScalarData Data = GetMtrlScalarData(_in.uInstID);
	
    VS_OUT _out = (VS_OUT) 0.f;
	
	switch (Data.MTRL_SCALAR_DEBUG_INT_SHAPETYPE)
	{
		case eDEBUGSHAPE_TYPE::RECT:
			_out.vPos = mul(float4(_in.vPos, 1.f), Data.MTRL_SCALAR_DEBUG_MAT_WORLD);
			_out.vPos = mul(float4(_out.vPos), g_CBuffer_matCam[g_SBuffer_Mtrl_Scalar[_in.uInstID].MTRL_SCALAR_INT_CAMIDX].matVP);
			break;
		
		case eDEBUGSHAPE_TYPE::CIRCLE:
			_out.vPos = mul(float4(_in.vPos, 1.f), Data.MTRL_SCALAR_DEBUG_MAT_WVP);
			break;
		
		case eDEBUGSHAPE_TYPE::CUBE:
			break;
		
		case eDEBUGSHAPE_TYPE::SPHERE:
			break;
		
		default:
			_out.vPos = (float4)0.f;
			break;
	};

	
	_out.uInstID = _in.uInstID;
	
	return _out;
}