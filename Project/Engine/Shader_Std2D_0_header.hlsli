#include "Shader_header_register.hlsli"

#define COLOR_KEY g_vec4_3

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
    float3 vPos : POSITION; // semantic  
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosSV : SV_Position;
    float2 vUV : TEXCOORD;
};

//SV_Position == Shader Variable Position. ���ؽ� ���̴��� �����Ͷ������� ���� ������ �ȼ��� ��ġ.



// ============================
// Std2DShader
// RasterizerState      : None
// BlendState           : Mask
// DepthStencilState    : Less
//
// Parameter
// g_int_0              : AnimUse
// g_vec2_0             : AnimAtlas LeftTop
// g_vec2_1             : AnimAtlas Slice
//
// g_tex_0              : Output Texture
// ============================
#define bAnimUse        g_int_0
#define LeftTop         g_vec2_0
#define Slice           g_vec2_1
#define Offset          g_vec2_2
#define CanvasSize        g_vec2_3