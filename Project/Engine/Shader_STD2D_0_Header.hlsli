#include "Shader_header_register.hlsli"


/*
�ʷϻ� ���ڴ� '�ø�ƽ'�̶�� �ϸ�,
���� �̸��� �ø�ƽ�� �ٸ� ���̴��� �Ѿ �� ���� �Ӽ��� ���� ���´�.
������ ���� ������ ���� �ƴ�. ���̴��� �߰� ������ ���� ����� ���� ���� �� �ִ�.
���� ���, ���� ���̴������� �����鿡 ���� ���길�� ����������, 
�ȼ� ���̴������� �����Ͷ������� ���ļ� ���� �ȿ� ���ϴ� �ȼ����� ����ŭ ����� �ϰ� �ȴ�.
���� POSITION �ø�ƽ�� �ȼ� ���̴����� ������ �� ���(������ ���޵ǰ� ���� ����)
���� ���̴��� POSITION �ø�ƽ�� ������ ��ġ�� ��ϵǾ� ������
�ȼ� ���̴������� ������ ���ؼ� �ľ��� �ȼ��� ��ġ�� ��ϵǾ� ���´�.
*/



struct VS_IN
{
    float3 vPos SEMANTIC(POSITION); // semantic  
    float2 vUV SEMANTIC(TEXCOORD);
    UINT32 uInstID SEMANTIC(SV_InstanceID);
};

struct VS_OUT
{
    float4 vPosSV SEMANTIC(SV_Position);
    float2 vUV SEMANTIC(TEXCOORD);
    UINT32 uInstID SEMANTIC(SV_InstanceID);
};

//SV_Position == Shader Variable Position. ���ؽ� ���̴��� �����Ͷ������� ���� ������ �ȼ��� ��ġ.



// ============================
// Std2DShader
// RasterizerState      : None
// BlendState           : Mask
// DepthStencilState    : Less
//
// Parameter
// g_CBuffer_Mtrl_Scalar.INT_0              : AnimUse
// g_CBuffer_Mtrl_Scalar.VEC2_0             : AnimAtlas LeftTop
// g_CBuffer_Mtrl_Scalar.VEC2_1             : AnimAtlas Slice
//
// g_tex_0              : Output Texture
// ============================
