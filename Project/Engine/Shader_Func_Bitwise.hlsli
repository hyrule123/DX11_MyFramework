#ifndef SHADER_FUNC_BITWISE
#define SHADER_FUNC_BITWISE

#include "Shader_Header_Register.hlsli"

//==========================================
//  �� ������ �ѷ� ������ �۾� �ÿ��� �ݵ�� ������� ����Ұ�
//  HLSL = ��Ʋ �����
//  �� �Լ��� ����ϴ� ��� : ���� ���� �����͸� ���ļ� �����ϴ� ���(UINT8 4�� �迭�� UINT32�� �����ϴ� ���)
//  
//==========================================

UINT32 UnpackUINT16FromUINT32_4(in UINT32_4 _u32Src, UINT32 _uIdxinU32)
{
	UINT32 result = 0u;
	
	//UINT32_4�� ���° �������� ���(��Ʈ�� 1��ŭ �������� �о��ָ� /2 �� ���� ����̴�)
	UINT32 Idx = _uIdxinU32 >> 1u;
	
	//��Ʈ�� �������� ���������� ���. 2�� ���� �������� �����ش�(1�� ��Ʈ &������ �ϸ� �������� ���Ҽ� ����)
	UINT32 FrontOrBack = _uIdxinU32 & 1u;
	
	result = _u32Src[Idx];
	

	result &= g_SBuffer_InitSettings[0].u16BitPartInU32Pack[FrontOrBack];
	result >>= g_SBuffer_InitSettings[0].u16BitShiftInU32Pack[FrontOrBack];

	
	return result;
}


#endif