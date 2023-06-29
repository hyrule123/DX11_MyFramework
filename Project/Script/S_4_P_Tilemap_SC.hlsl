#include "S_H_Tilemap_SC.hlsli"

float4 PS_Tilemap_SC(VS_OUT _in) : SV_TARGET
{
	float4 vOutColor = (float4) 0.f;
	
	if (FALSE == g_CBuffer_Mtrl_Tex.bTEX_0)
		return float4(1.f, 0.f, 1.f, 1.f);
	
	vOutColor = g_tex_0.Sample(g_Sampler_0, _in.vUV);
	
	int DebugMode = g_CBuffer_Mtrl_Scalar.MTRL_SCALAR_INT_DEBUGMODE;
	
	if (DEBUGMODE_MEGATILE == DebugMode)
	{
		//아래 3개는 SCMapLoader에 정의되어 있음.
		//#define MTRL_SCALAR_VEC2_MAPSIZE	  MTRLDATA_PARAM_SCALAR(VEC2, 0)
		//#define MTRL_SCALAR_VEC2_MINITILESIZE MTRLDATA_PARAM_SCALAR(VEC2, 1)
		//#define MTRL_SCALAR_VEC2_MEGATILESIZE MTRLDATA_PARAM_SCALAR(VEC2, 2)

		int2 MyMegatilePos = (int2) (g_CBuffer_Mtrl_Scalar.MTRL_SCALAR_VEC2_MEGATILESIZE * _in.vUV);
		
		int index = MyMegatilePos.y * g_CBuffer_SBufferData[eCBUFFER_SBUFFER_SHAREDATA_IDX::TILE].MEGA_TILE_SIZE_X
		 + MyMegatilePos.x;
	
		if (g_SBuffer_MegaTile[index].bBuildUnable == TRUE)
			vOutColor *= float4(1.f, 0.f, 0.f, 1.f);
	}
	else if (DEBUGMODE_MINITILE == DebugMode)
	{
		int2 MyMinitilePos = (int2) (g_CBuffer_Mtrl_Scalar.MTRL_SCALAR_VEC2_MINITILESIZE * _in.vUV);
		
		int index = MyMinitilePos.y * g_CBuffer_SBufferData[eCBUFFER_SBUFFER_SHAREDATA_IDX::TILE].MEGA_TILE_SIZE_X * 8u
		 + MyMinitilePos.x;
	
		if (g_SBuffer_MiniTile[index].bWalkable == FALSE)
			vOutColor *= float4(1.f, 0.f, 1.f, 1.f);
	}
	
	return vOutColor;
}
