#include "Shader_header_register.hlsli"


struct VS_INOUT
{
    float3 vPos : POSITION;
    uint iInstID : SV_InstanceID;
};

//Geometry Shader 의 사용 목적
//1. 파이프라인 제어(픽셀 쉐이더에 도달하기 전에 쉐이더 파이프라인을 중단)
//2. 빌보드 처리(어떤 상태에서든 카메라를 바라봄)
struct GS_OUT
{
    float4 vSV_Pos : SV_Position;
    float2 vUV : TEXCOORD;
};