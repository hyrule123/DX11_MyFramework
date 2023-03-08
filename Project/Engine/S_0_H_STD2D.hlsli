#include "S_H_Register.hlsli"

#ifndef __cplusplus
#include "S_H_Func.hlsli"
#endif

/*
초록색 숫자는 '시맨틱'이라고 하며,
같은 이름의 시맨틱은 다른 쉐이더로 넘어갈 때 같은 속성의 값이 들어온다.
동일한 값이 들어오는 것은 아님. 쉐이더가 추가 연산을 통해 변경된 값이 들어올 수 있다.
예를 들어, 정점 쉐이더에서는 정점들에 대한 연산만을 진행하지만, 
픽셀 쉐이더에서는 래스터라이저를 거쳐서 정점 안에 속하는 픽셀들의 수만큼 계산을 하게 된다.
만약 POSITION 시맨틱이 픽셀 쉐이더까지 전달이 될 경우(지금은 전달되고 있지 않음)
정점 쉐이더는 POSITION 시맨틱의 정점의 위치가 기록되어 있지만
픽셀 쉐이더에서는 보간을 통해서 파악한 픽셀의 위치가 기록되어 들어온다.
*/


//인스턴싱 : 사용
//알파 블렌딩 : ON




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

//SV_Position == Shader Variable Position. 버텍스 쉐이더와 래스터라이저를 거쳐 결정된 픽셀의 위치.



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
