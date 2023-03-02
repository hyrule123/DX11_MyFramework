#ifndef SHADER_FUNC_BITWISE
#define SHADER_FUNC_BITWISE

#include "Shader_Header_Register.hlsli"

//==========================================
//  �� ������ �ѷ� ������ �۾� �ÿ��� �ݵ�� ������� ����Ұ�
//  HLSL = ��Ʋ �����
//  �� �Լ��� ����ϴ� ��� : ���� ���� �����͸� ���ļ� �����ϴ� ���(UINT8 4�� �迭�� UINT32�� �����ϴ� ���)
//  
//==========================================

//_uIdxinU16 ���� : 0 ~ 7
UINT32 UnpackUINT16FromUINT32_4(in UINT32_4 _u3_42Src, UINT32 _uIdxinU16)
{
	UINT32 result = 0u;
	
	//UINT32_4�� ���° �������� ���(��Ʈ�� 1��ŭ �������� �о��ָ� /2 �� ���� ����̴�)
	UINT32 Idx = (_uIdxinU16 >> 1u) & 0x07;
	
	//��Ʈ�� �������� ���������� ���. 2�� ���� �������� �����ش�(1�� ��Ʈ &������ �ϸ� �������� ���Ҽ� ����)
	UINT32 FrontOrBack = _uIdxinU16 & 1u;
	
	result = _u3_42Src[Idx];

	result &= g_SBuffer_InitSettings[0].u16BitPartInU32Pack[FrontOrBack];
	result >>= g_SBuffer_InitSettings[0].u16BitShiftInU32Pack[FrontOrBack];

	
	return result;
}

//_uIdxInU16 ���� : 0 ~ 15
UINT32 UnpackUINT16FromUINT32_8(in UINT32_8 _u32_8Src, UINT32 _uIdxInU16)
{
	UINT32 result = 0u;
	
	//row == 8�� ���� ���, 0 �Ǵ� 1
	UINT32 row = (_uIdxInU16 >> 3u) & 0x1;
	
	//col == 8�� ���� �������� 2�� �����ش�.
	UINT32 col = (_uIdxInU16 & 7u) >> 1u;
	
	//�պκ����� �޺κ����� Ȯ��(2�� ���� ������)
	UINT32 FrontOrBack = _uIdxInU16 & 0x1;
	
	result = (_u32_8Src[row][col] & g_SBuffer_InitSettings[0].u16BitPartInU32Pack[FrontOrBack]) >> g_SBuffer_InitSettings[0].u16BitShiftInU32Pack[FrontOrBack];
	//& g_SBuffer_InitSettings[0].u16BitPartInU32Pack[FrontOrBack]) >> g_SBuffer_InitSettings[0].u16BitShiftInU32Pack[FrontOrBack];
	
	return result;
}

//_uIdxInU16 ���� : 0 ~ 3
UINT32 UnpackUINT8FromUINT32(in UINT32 _u32Src, UINT32 _uIdxInU8)
{
	UINT32 result = 0u;
	
	//0~3 ������ ������ ����
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


#endif