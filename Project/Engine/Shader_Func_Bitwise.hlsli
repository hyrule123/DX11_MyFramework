#ifndef SHADER_FUNC_BITWISE
#define SHADER_FUNC_BITWISE

#include "Shader_header_struct.hlsli"

//==========================================
//  �� ������ �ѷ� ������ �۾� �ÿ��� �ݵ�� ������� ����Ұ�
//  HLSL = ��Ʋ �����
//==========================================

UINT32_2 ExtractUINT16FromUINT32_ALL(UINT32 _u32)
{
	UINT32_2 result = (UINT32_2) 0u;
    
	//��Ʋ ����� ����̹Ƿ� ���� ������ ���� = ����� �������� ����
	result.x = (0x0000ffff & _u32);
	result.y = (0xffff0000 & _u32) >> 16u;
    
	return result;
}

//0 == �պκ�, 1 = �޺κ� 
UINT32 ExtractUINT16FromUINT32(UINT32 _u32, UINT32 _uIdx)
{
	UINT32 res = (UINT32) 0u;
	
	_uIdx *= 16u;
	
	res = ((0xffff0000 >> (16u - _uIdx)) & _u32) >> _uIdx;
	
	return res;
}


//UINT32_4(16����Ʈ)�� 8�κ����� �ɰ��� ���� �κ�(=2byte, 16bit ����) �����ش�.
//_uIdx ���� : 0 ~ 7
UINT32 ExtractUINT16FromUINT32_4(UINT32_4 _u32_4, UINT32 _uIdx)
{
	UINT32 res = 0u;
	
	//uint�� �ε��� ��ȣ -> ��Ʈ����Ʈ 1���ָ� 2�� ���� ȿ��
	UINT32 Idx = _uIdx >> 1u; 
	
	//�պκ����� �޺κ����� : 2�� ���� ������ ( & ������ )
	UINT32 FrontOrBack = (_uIdx & 0x00000001) * 16u;
	
	res = ((0xFFFF0000 >> (16u - FrontOrBack)) & _u32_4[Idx]) >> FrontOrBack;
	
	return res;
}

UINT32_2 ExtractUINT16FromUINT32_4_ALL(UINT32_4 _u32_4, UINT32 _uIdx)
{
	UINT32_2 res = 0u;
	
	//uint�� �ε��� ��ȣ -> ��Ʈ����Ʈ 1���ָ� 2�� ���� ȿ��
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
//3 �̻��� ����
UINT32 ExtractUINT8FromUINT32(UINT32 _u32, UINT32 _Index)
{
	UINT32 res = 0u;
    
    //��Ʈ ����ŭ �����ش�
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