
#include "S_0_H_SCMapLoader.hlsli"

#include "S_H_FuncBitwise.hlsli"

uint ExtractGroupAndIndexFromMTXM(in uint2 _uGroupID_XY, in uint2 _u2MapSize);
uint ExtractWPEColorIdxFromVR4(in uint _uMiniTileIdx, in uint2 _u2GroupThreadID, in bool _bFlipped);
//uint4 GetColorFromWPE(in uint Idx);


[numthreads(32, 32, 1)]
void CS_SCMapLoader(uint3 _uDTID : SV_DispatchThreadID, uint3 _uGTID : SV_GroupThreadID,
uint3 _uGroupID : SV_GroupID)
{
	uint2 u2MapSize = (uint2)round(g_CBuffer_Mtrl_Scalar.MTRL_SCALAR_VEC2_MAPSIZE);
	
	//uint2 u2MapSize = uint2(
	//			(uint) g_CBuffer_Mtrl_Scalar.MTRL_SCALAR_CS_TOTAL_ELEMCOUNT_X / 32u,
	//			(uint) g_CBuffer_Mtrl_Scalar.MTRL_SCALAR_CS_TOTAL_ELEMCOUNT_Y / 32u);
	
	uint GroupAndIndex = ExtractGroupAndIndexFromMTXM(_uGroupID.xy, u2MapSize);
	
	//그룹과 인덱스 번호를 구해준다.
	uint group = GroupAndIndex >> 4u;
	uint index = GroupAndIndex & 0x0000000Fu;
	
	uint megatile = UnpackUINT16FromUINT32_8(g_SBuffer_CV5[group].MegaTileIndex, index);
	
	//자신이 메가타일에서 어느 미니타일인지를 계산한다.
	uint MyGTIdIdx = (_uGTID.y / 8u) * 4u + _uGTID.x / 8u;
	uint UnpackedVX4 = UnpackUINT16FromUINT32_8(g_SBuffer_VX4[megatile].VR4Index, MyGTIdIdx);
	
	uint minitileindex = UnpackedVX4 >> 1u;
	bool flipped = (UnpackedVX4 & 1u == 1u);
	
	//VF4는 일단 보류(전달할 데이터 버퍼가 없음)
	
	//미니타일의 8 * 8을 순회돌면서 렌더링
	//자신의 인덱스 번호를 구한다.
	uint idxInWPE = ExtractWPEColorIdxFromVR4(minitileindex, _uGTID.xy, flipped);
	
	
	uint IdxInPack = idxInWPE % 4u;
	uint IdxWPE = idxInWPE / 4u;
	
	uint4 u4Color = UnpackUINT8FromUINT32_ALL(g_SBuffer_WPE[IdxWPE].RGBAPack[IdxInPack]);
	u4Color.w = 255;
	
	float4 f4Color = (float4) u4Color;
	f4Color /= 255.f;
	
	g_TexRW_SCMap[_uDTID.xy] = f4Color;

	//if (all(_uDTID == _uGroupID * 32u))
	//{
	//	uint originalval = 0u;
	//	InterlockedExchange(g_SBufferRW_Debug[_uGroupID.y * u2MapSize.x + _uGroupID.x].INT_0, (int) group, originalval);
	
	//	InterlockedExchange(g_SBufferRW_Debug[_uGroupID.y * u2MapSize.x + _uGroupID.x].INT_1, (int) index, originalval);
	
	//	InterlockedExchange(g_SBufferRW_Debug[_uGroupID.y * u2MapSize.x + _uGroupID.x].INT_2, (int) GroupAndIndex, originalval);
		
		
		
	//	//InterlockedExchange(g_SBufferRW_Debug[_uGroupID.y * u2MapSize.x + _uGroupID.x].INT_3, (int) megatile, originalval);
	//}

	if(all(_uDTID == uint3(0, 32, 0)))
	{
		uint originalval = 0u;
		InterlockedExchange(g_SBufferRW_Debug[0].INT_0, (int) u4Color.x, originalval);
		InterlockedExchange(g_SBufferRW_Debug[0].INT_1, (int) u4Color.y, originalval);
		InterlockedExchange(g_SBufferRW_Debug[0].INT_2, (int) u4Color.z, originalval);
		InterlockedExchange(g_SBufferRW_Debug[0].INT_3, (int) u4Color.w, originalval);
		
		InterlockedExchange(g_SBufferRW_Debug[0].FLOAT_0, (float) f4Color.x, originalval);
		InterlockedExchange(g_SBufferRW_Debug[0].FLOAT_1, (float) f4Color.y, originalval);
		InterlockedExchange(g_SBufferRW_Debug[0].FLOAT_2, (float) f4Color.z, originalval);
		InterlockedExchange(g_SBufferRW_Debug[0].FLOAT_3, (float) f4Color.w, originalval);
	}
	
	

}


uint ExtractGroupAndIndexFromMTXM(in uint2 _uGroupID_XY, in uint2 _u2MapSize)
{
	uint GroupAndIndex = 0u;
	
	uint uGroupID_Xdiv8 = _uGroupID_XY.x / MXTM_PACK;
	uint MapSizeDiv8 = _u2MapSize.x / MXTM_PACK;
	
	uint IdxInPack = _uGroupID_XY.x % MXTM_PACK;
	
	GroupAndIndex = UnpackUINT16FromUINT32_4(g_SBuffer_MXTM[_uGroupID_XY.y * MapSizeDiv8.x + uGroupID_Xdiv8].MXTM_Pack, IdxInPack);
	
	
	return GroupAndIndex;
}


uint ExtractWPEColorIdxFromVR4(in uint _uMiniTileIdx, in uint2 _u2GroupThreadID, in bool _bFlipped)
{
	uint result = 0u;
	
	//나머지 연산을 통해 자신의 미니타일 안에서의 인덱스 번호를 구한다.
	uint2 myIdxInMinitile = _u2GroupThreadID % 8u;
	
	if(true == _bFlipped)
		myIdxInMinitile.x = 7u - myIdxInMinitile.x;
	
	//1차원 배열에서의 번호
	uint arrIdx = myIdxInMinitile.y * 8u + myIdxInMinitile.x;
	
	//4 by 4 상태로 변환한다. (8 by 8 -> 4 by 4)
	uint2 IdxIn4x4 = uint2((arrIdx % 16u) / 4u, arrIdx / 16u);
		
	//위 결과가 곧 4by4 행렬의 인덱스 번호이다.
	uint ColorPack = g_SBuffer_VR4[_uMiniTileIdx].ColorPack[IdxIn4x4.y][IdxIn4x4.x];
	
	//UINT32 내부에서의 인덱스 번호도 필요하다.
	uint IdxInColorPack = arrIdx % 4u;
	
	//UINT32에서 UINT8값을 추출한다.
	result = UnpackUINT8FromUINT32(ColorPack, IdxInColorPack);
	
	return result;
}

