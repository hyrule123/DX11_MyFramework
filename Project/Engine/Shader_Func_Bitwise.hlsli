#ifndef SHADER_FUNC_BITWISE
#define SHADER_FUNC_BITWISE

#include "Shader_header_struct.hlsli"

//==========================================
//  한 변수를 둘로 나누는 작업 시에는 반드시 엔디안을 고려할것
//  HLSL = 리틀 엔디안
//==========================================

UINT32_2 ExtractUINT16FromUINT32_ALL(UINT32 _u32)
{
	UINT32_2 result = (UINT32_2) 0u;
    
	//리틀 엔디안 방식이므로 실제 변수의 앞쪽 = 저장된 데이터의 뒷쪽
	result.x = (0x0000ffff & _u32);
	result.y = (0xffff0000 & _u32) >> 16u;
    
	return result;
}

//0 == 앞부분, 1 = 뒷부분 
UINT32 ExtractUINT16FromUINT32(UINT32 _u32, UINT32 _uIdx)
{
	UINT32 res = (UINT32) 0u;
	
	_uIdx *= 16u;
	
	res = ((0xffff0000 >> (16u - _uIdx)) & _u32) >> _uIdx;
	
	return res;
}


//UINT32_4(16바이트)를 8부분으로 쪼갰을 때의 부분(=2byte, 16bit 단위) 구해준다.
//_uIdx 범위 : 0 ~ 7
UINT32 ExtractUINT16FromUINT32_4(UINT32_4 _u32_4, UINT32 _uIdx)
{
	UINT32 res = 0u;
	
	//uint의 인덱스 번호 -> 비트시프트 1해주면 2로 나눈 효과
	UINT32 Idx = _uIdx >> 1u; 
	
	//앞부분인지 뒷부분인지 : 2로 나눈 나머지 ( & 연산자 )
	UINT32 FrontOrBack = (_uIdx & 0x00000001) * 16u;
	
	res = ((0xFFFF0000 >> (16u - FrontOrBack)) & _u32_4[Idx]) >> FrontOrBack;
	
	return res;
}

UINT32_2 ExtractUINT16FromUINT32_4_ALL(UINT32_4 _u32_4, UINT32 _uIdx)
{
	UINT32_2 res = 0u;
	
	//uint의 인덱스 번호 -> 비트시프트 1해주면 2로 나눈 효과
	UINT32 Idx = _uIdx >> 1u;
	
	res.x = 0x0000ffff & _u32_4[Idx];
	res.y = (0xffff0000 & _u32_4[Idx]) >> 16u;
	
	
	return res;
}


UINT32_4 ExtractUINT8FromUINT32_ALL(UINT32 _u32)
{
	UINT32_4 result = (UINT32_4) 0u;
    
	result.x = (0x000000ff & _u32);
	
	result.y = (0x0000ff00 & _u32) >> 8u;
	
	result.z = (0x00ff0000 & _u32) >> 16u;
	
	result.w = (0xff000000 & _u32) >> 24u;
    
	
	return result;
}

//Index 0 = 0xff000000, 1 = 0x00ff0000, 2 = 0x0000ff00, 3 = 0x000000ff
//3 이상은 에러
UINT32 ExtractUINT8FromUINT32(UINT32 _u32, UINT32 _Index)
{
	UINT32 res = 0u;
    
    //비트 수만큼 곱해준다
	_Index *= 8u;
    
	res = ((0xff000000 >> (24u - _Index)) & _u32) >> _Index;
    
	return res;
}


UINT32_4 ExtractRGBA(UINT32 _u32RGBA)
{
	UINT32_4 RGBA = (UINT32_4) 0;
    
	RGBA.a = (_u32RGBA & 0xFF000000);
	RGBA.a = RGBA.r >> 24;
    
	RGBA.r = (_u32RGBA & 0x00FF0000);
	RGBA.r = RGBA.g >> 16;
    
	RGBA.g = (_u32RGBA & 0x0000FF00);
	RGBA.g = RGBA.b >> 8;
    
	RGBA.b = (_u32RGBA & 0x000000FF);
    
	return RGBA;
}


#endif