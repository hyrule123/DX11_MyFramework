#include "Shader_header_register.hlsli"


struct VS_INOUT
{
    float3 vPos : POSITION;
    uint iInstID : SV_InstanceID;
};

//Geometry Shader �� ��� ����
//1. ���������� ����(�ȼ� ���̴��� �����ϱ� ���� ���̴� ������������ �ߴ�)
//2. ������ ó��(� ���¿����� ī�޶� �ٶ�)
struct GS_OUT
{
    float4 vSV_Pos : SV_Position;
    float2 vUV : TEXCOORD;
};