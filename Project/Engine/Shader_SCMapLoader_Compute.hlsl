
#include "Shader_SCMapLoader_0_Header.hlsli"

#include "Shader_Func_Bitwise.hlsli"

uint ExtractGroupAndIndexFromMTXM(uint2 _uGroupID_XY, uint2 _u2MapSize);

[numthreads(32, 32, 1)]
void CS_SCMapLoader( uint3 _uDTID : SV_DispatchThreadID, uint3 _uGroupID : SV_GroupID )
{
	uint2 u2MapSize = uint2(
				(uint) g_CBuffer_Mtrl_Scalar.MTRL_SCALAR_CS_TOTAL_ELEMCOUNT_X / 32u,
				(uint) g_CBuffer_Mtrl_Scalar.MTRL_SCALAR_CS_TOTAL_ELEMCOUNT_Y / 32u);
	
	uint GroupAndIndex = ExtractGroupAndIndexFromMTXM(_uGroupID.xy, u2MapSize);
	
	uint group = GroupAndIndex >> 4;
	uint index = GroupAndIndex & 0x0000000f;
	

	//uint originalval = 0u;
	//InterlockedExchange(g_SBufferRW_Debug[_uGroupID.y * u2MapSize.x + _uGroupID.x].INT_0, (int) group, originalval);
	
	//InterlockedExchange(g_SBufferRW_Debug[_uGroupID.y * u2MapSize.x + _uGroupID.x].INT_1, (int) index, originalval);
	
	//InterlockedExchange(g_SBufferRW_Debug[_uGroupID.y * u2MapSize.x + _uGroupID.x].INT_2, (int) GroupAndIndex, originalval);
	

	if(all(uint3(0u, 0u, 0u) == _uDTID))
	{
		uint GroupAndIndex = (uint2) 0;
	
		uint uGroupID_Xdiv8 = _uGroupID.x / MXTM_PACK;
		uint MapSizeDiv8 = u2MapSize / MXTM_PACK;
	
		uint IdxInPack = _uGroupID.x % MXTM_PACK;
		
		g_SBufferRW_Debug[0].INT_0 = (int) uGroupID_Xdiv8;
		g_SBufferRW_Debug[0].INT_1 = (int) MapSizeDiv8;
		g_SBufferRW_Debug[0].INT_2 = (int) IdxInPack;

	}

}


uint ExtractGroupAndIndexFromMTXM(uint2 _uGroupID_XY, uint2 _u2MapSize)
{
	uint GroupAndIndex = (uint2) 0;
	
	uint uGroupID_Xdiv8 = _uGroupID_XY.x / MXTM_PACK;
	uint MapSizeDiv8 = _u2MapSize.x / MXTM_PACK;
	
	uint IdxInPack = _uGroupID_XY.x % MXTM_PACK;
	
	GroupAndIndex = ExtractUINT16FromUINT32_4(g_SBuffer_MXTM[_uGroupID_XY.y * MapSizeDiv8.x + uGroupID_Xdiv8].MXTM_Pack, IdxInPack);
	
	
	return GroupAndIndex;
}