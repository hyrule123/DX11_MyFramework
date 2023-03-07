#include "Shader_TilemapAtlas_0_header.hlsli"

VTX_TILEMAP_OUT VS_Tilemap_Atlas(VTX_TILEMAP_IN _in)
{
    VTX_TILEMAP_OUT output = (VTX_TILEMAP_OUT) 0.f;
    

	output.vPosition = mul(float4(_in.vPos, 1.f), g_CBuffer_Mtrl_Scalar.MTRL_SCALAR_MAT_WORLD);
	output.vPosition = mul(float4(output.vPosition), g_CBuffer_matCam[g_CBuffer_Mtrl_Scalar.MTRL_SCALAR_INT_CAMIDX].matVP);
    output.vUV = _in.vUV * float2(g_CBuffer_Mtrl_Scalar.INT_0, g_CBuffer_Mtrl_Scalar.INT_1); // 전체 UV 를 타일 개수만큼 확장 
    
    return output;
}