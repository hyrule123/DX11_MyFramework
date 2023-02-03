#include "Shader_header_struct.hlsli"


cbuffer eCOMPONENT_TRANSFORM : register(b0)
{
    matrix g_matWorld;
    matrix g_matWVP;
};

cbuffer eCONST_BUFFER_MATERIAL : register(b1)
{
    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;
    
    float g_float_0;
    float g_float_1;
    float g_float_2;
    float g_float_3;
    
    float2 g_vec2_0;
    float2 g_vec2_1;
    float2 g_vec2_2;
    float2 g_vec2_3;

    float4 g_vec4_0;
    float4 g_vec4_1;
    float4 g_vec4_2;
    float4 g_vec4_3;
    
    matrix g_mat_0;
    matrix g_mat_1;
    matrix g_mat_2;
    matrix g_mat_3;
    
    
    //�ؽ�ó�� ������ �Ǵ��ϱ����� ����
    int g_btex_0;
    int g_btex_1;
    int g_btex_2;
    int g_btex_3;
    int g_btex_4;
    int g_btex_5;
    int g_btex_6;
    int g_btex_7;
};

//������ ���� ������ �Ѱ��ֱ� ���� �������
cbuffer tGlobalValue : register(b2)
{
    float2 g_Resolution;
    float g_DeltaTime;
    float g_AccTime;
};


//�ϳ��� ����ȭ ���۰� �����ϴ� �ڿ��� ���� ������ ����
//�ε��� ��ȣ = ����ȭ ������ �������� ��ȣ
cbuffer SBufferInfo : register(b3)
{
    tSBufferInfo g_SBufferInfo[eSBUFFER_SHARED_CBUFFER_END];
}


Texture2D g_tex_0 : register(t0);
Texture2D g_tex_1 : register(t1);
Texture2D g_tex_2 : register(t2);
Texture2D g_tex_3 : register(t3);
Texture2D g_tex_4 : register(t4);
Texture2D g_tex_5 : register(t5);
Texture2D g_tex_6 : register(t6);
Texture2D g_tex_7 : register(t7);

StructuredBuffer<tLightInfo> g_Light2DSBuffer : register(t8);
StructuredBuffer<tTile> g_TileSBuffer : register(t9);
StructuredBuffer<float4> g_TestBuffer : register(t10);

SamplerState g_sam_0 : register(s0);
SamplerState g_sam_1 : register(s1);