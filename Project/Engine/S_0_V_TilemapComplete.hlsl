#include "S_H_TilemapComplete.hlsli"

VS_OUT VS_Tilemap_Complete(VS_IN _in)
{
	VS_OUT _out = (VS_OUT) 0;
	
	_out.vWorldPos = mul(float4(_in.vLocalPos, 1.f), g_CBuffer_Mtrl_Scalar.MTRL_SCALAR_MAT_WORLD);
	_out.vWorldPos = mul(_out.vWorldPos, g_CBuffer_matCam[g_CBuffer_Mtrl_Scalar.MTRL_SCALAR_INT_CAMIDX].matVP);
	
	_out.vUV = _in.vUV;
	
	return _out;
}
