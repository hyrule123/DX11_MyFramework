#include "S_H_Tilemap_SC.hlsli"

float4 PS_Tilemap_SC(VS_OUT _in) : SV_TARGET
{
	float4 vOutColor = (float4) 0.f;
	
	if (FALSE == g_CBuffer_Mtrl_Tex.bTEX_0)
		return float4(1.f, 0.f, 1.f, 1.f);
	
	//if (g_CBuffer_SBuffer_ShareData[eCBUFFER_SBUFFER_SHAREDATA_IDX::TILE].iData0 != 0)
	//	return float4(1.f, 1.f, 1.f, 1.f);
	float2 MapSizeXY = float2((float) g_CBuffer_SBuffer_ShareData[eCBUFFER_SBUFFER_SHAREDATA_IDX::TILE].MEGA_TILE_SIZE_X,
	(float) g_CBuffer_SBuffer_ShareData[eCBUFFER_SBUFFER_SHAREDATA_IDX::TILE].MEGA_TILE_SIZE_Y);
	
	//미니타일 갯수
	MapSizeXY *= 4;
	
	int minitilerownum = (int) MapSizeXY.x;
	
	MapSizeXY *= _in.vUV;
	
	int index = (int) MapSizeXY.x + (int) MapSizeXY.y * minitilerownum;
	
	if (g_SBuffer_MiniTile[index].bWalkable == TRUE)
		return float4(1.f, 0.f, 1.f, 1.f);
	
	return g_tex_0.Sample(g_Sampler_0, _in.vUV);
}
