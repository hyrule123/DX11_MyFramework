#ifndef SHADER_FUNC_BITWISE
#define SHADER_FUNC_BITWISE

#include "Shader_Header_Register.hlsli"

//==========================================
//  한 변수를 둘로 나누는 작업 시에는 반드시 엔디안을 고려할것
//  HLSL = 리틀 엔디안
//  이 함수를 사용하는 경우 : 여러 개의 데이터를 뭉쳐서 전달하는 경우(UINT8 4개 배열을 UINT32로 전달하는 경우)
//  
//==========================================

UINT32 UnpackUINT16FromUINT32_4(in UINT32_4 _u32Src, UINT32 _uIdxinU32)
{
	UINT32 result = 0u;
	
	//UINT32_4의 몇번째 값인지를 계산(비트를 1만큼 우측으로 밀어주면 /2 와 같은 결과이다)
	UINT32 Idx = _uIdxinU32 >> 1u;
	
	//비트의 앞쪽인지 뒤쪽인지를 계산. 2로 나눈 나머지를 구해준다(1과 비트 &연산을 하면 나머지를 구할수 있음)
	UINT32 FrontOrBack = _uIdxinU32 & 1u;
	
	result = _u32Src[Idx];
	

	result &= g_SBuffer_InitSettings[0].u16BitPartInU32Pack[FrontOrBack];
	result >>= g_SBuffer_InitSettings[0].u16BitShiftInU32Pack[FrontOrBack];

	
	return result;
}


#endif