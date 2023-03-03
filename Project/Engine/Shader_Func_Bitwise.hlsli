#ifndef SHADER_FUNC_BITWISE
#define SHADER_FUNC_BITWISE

#include "Shader_Header_Register.hlsli"

//==========================================
//  한 변수를 둘로 나누는 작업 시에는 반드시 엔디안을 고려할것
//  HLSL = 리틀 엔디안
//  이 함수를 사용하는 경우 : 여러 개의 데이터를 뭉쳐서 전달하는 경우(UINT8 4개 배열을 UINT32로 전달하는 경우)
//  
//==========================================

//_uIdxinU16 범위 : 0 ~ 7
UINT32 UnpackUINT16FromUINT32_4(in UINT32_4 _u3_42Src, UINT32 _uIdxinU16)
{
	UINT32 result = 0u;
	
	//UINT32_4의 몇번째 값인지를 계산(비트를 1만큼 우측으로 밀어주면 /2 와 같은 결과이다)
	UINT32 Idx = (_uIdxinU16 >> 1u) & 0x07;
	
	//비트의 앞쪽인지 뒤쪽인지를 계산. 2로 나눈 나머지를 구해준다(1과 비트 &연산을 하면 나머지를 구할수 있음)
	UINT32 FrontOrBack = _uIdxinU16 & 1u;
	
	result = _u3_42Src[Idx];

	result &= g_SBuffer_InitSettings[0].u16BitPartInU32Pack[FrontOrBack];
	result >>= g_SBuffer_InitSettings[0].u16BitShiftInU32Pack[FrontOrBack];

	
	return result;
}

//_uIdxInU16 범위 : 0 ~ 15
UINT32 UnpackUINT16FromUINT32_8(in UINT32_8 _u32_8Src, UINT32 _uIdxInU16)
{
	UINT32 result = 0u;
	
	//row == 8로 나눈 결과, 0 또는 1
	UINT32 row = (_uIdxInU16 >> 3u) & 0x1;
	
	//col == 8로 나눈 나머지를 2로 나눠준다.
	UINT32 col = (_uIdxInU16 & 7u) >> 1u;
	
	//앞부분인지 뒷부분인지 확인(2로 나눈 나머지)
	UINT32 FrontOrBack = _uIdxInU16 & 0x1;
	
	result = (_u32_8Src[row][col] & g_SBuffer_InitSettings[0].u16BitPartInU32Pack[FrontOrBack]) >> g_SBuffer_InitSettings[0].u16BitShiftInU32Pack[FrontOrBack];
	//& g_SBuffer_InitSettings[0].u16BitPartInU32Pack[FrontOrBack]) >> g_SBuffer_InitSettings[0].u16BitShiftInU32Pack[FrontOrBack];
	
	return result;
}

//_uIdxInU16 범위 : 0 ~ 3
UINT32 UnpackUINT8FromUINT32(in UINT32 _u32Src, UINT32 _uIdxInU8)
{
	UINT32 result = 0u;
	
	//0~3 까지의 범위로 한정
	_uIdxInU8 &= 0x03;
	
	result = (_u32Src & g_SBuffer_InitSettings[0].u8BitPartInU32Pack[_uIdxInU8]) >> g_SBuffer_InitSettings[0].u8BitShiftInU32Pack[_uIdxInU8];
	
	return result;
}


UINT32_4 UnpackUINT8FromUINT32_ALL(in UINT32 _u32Src)
{
	UINT32_4 result = (UINT32_4) 0u;


	result.r = (_u32Src & g_SBuffer_InitSettings[0].u8BitPartInU32Pack[0]) 
		>> g_SBuffer_InitSettings[0].u8BitShiftInU32Pack[0];
	
	result.g = (_u32Src & g_SBuffer_InitSettings[0].u8BitPartInU32Pack[1])
		>> g_SBuffer_InitSettings[0].u8BitShiftInU32Pack[1];
	
	result.b = (_u32Src & g_SBuffer_InitSettings[0].u8BitPartInU32Pack[2])
		>> g_SBuffer_InitSettings[0].u8BitShiftInU32Pack[2];
	
	result.a = (_u32Src & g_SBuffer_InitSettings[0].u8BitPartInU32Pack[3])
		>> g_SBuffer_InitSettings[0].u8BitShiftInU32Pack[3];
	
	return result;
}

UINT32_2 UnpackUINT16FromUINT32_ALL(in UINT32 _u32Src)
{
	UINT32_2 result = (UINT32_2) 0u;

	result.x = (_u32Src & g_SBuffer_InitSettings[0].u16BitPartInU32Pack[0]) >> g_SBuffer_InitSettings[0].u16BitShiftInU32Pack[0];
	
	result.y = (_u32Src & g_SBuffer_InitSettings[0].u16BitPartInU32Pack[1]) >> g_SBuffer_InitSettings[0].u16BitShiftInU32Pack[1];
	
	return result;
}



#endif