#ifndef SHADER_FUNC
#define SHADER_FUNC

#include "Shader_header_register.hlsli"

uint XorShift64Plus(inout uint2 state)
{
	uint x = state.x;
	uint y = state.y;
	state.x = y;
	x ^= x << 23;
	state.y = x ^ y ^ (x >> 17) ^ (y >> 26);
	return state.y + y;
}

static float GaussianFilter[5][5] =
{
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.0219f, 0.0983f, 0.1621f, 0.0983f, 0.0219f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
};

#define PI 3.14159265f
#define PI_DOUBLE 6.2831853f

void GaussianSample(in Texture2D _Tex_Noise, float2 _vResolution, float _NormalizedThreadID, out float3 _vOut)
{
    //들어온 정규화된 스레드 ID를 UV값으로 사용한다.
    float2 vUV = float2(_NormalizedThreadID, 0.5f);
    
    //x도 약간의 랜덤성을 넣기 위해 지속적으로 움직어준다.
    vUV.x += g_CBuffer_GlobalData.fAccTime * 0.5f;
    
    // sin 그래프로 텍스쳐의 샘플링 위치 UV 를 계산
    //fAccTime : 게임 실행 후 누적된 게임 실행 시간. 이 값을 활용하면 사인그래프를 지속적으로 우측으로 이동시킬 수 있다.
    //
    vUV.y -= (sin((_NormalizedThreadID - (g_CBuffer_GlobalData.fAccTime /*그래프 우측 이동 속도*/)) * PI_DOUBLE * 10.f /*반복주기*/) * 0.5f);
    
    //UV값이 0 ~ 1 사이의 값을 벗어났을 경우 사이로 보정해준다. 난수까지는 아니여도 약간의 불규칙성을 가진 유사난수가 만들어짐.
    if (1.f < vUV.x)
        vUV.x = frac(vUV.x);
    else if (vUV.x < 0.f)
        vUV.x = 1.f + frac(vUV.x);
    
    if (1.f < vUV.y)
        vUV.y = frac(vUV.y);
    else if (vUV.y < 0.f)
        vUV.y = 1.f + frac(vUV.y);
    
    //만들어진 UV를 텍스처의 사이즈에 곱해 주면 추출할 픽셀의 인덱스 번호를 얻을 수 있다.
    int2 pixel = vUV * _vResolution;
    
    //5 * 5짜리 가우시안 필터를 중점(2, 2)으로 적용하기 위해서 골라진 픽셀에서 3칸씩 밀어준다.
    //
    // 0 1 2 3 4    
    // 1
    // 2  (2, 2) = 중점
    // 3
    // 4
    const static int2 offset = int2(-2, -2);
    float3 vOut = (float3) 0.f;
    
    //순회를 돌아주면서 해당 인덱스의 색상에 가우시안 필터 값을 곱해서 최종 값을 합산한다.
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            vOut += (_Tex_Noise[pixel + offset + int2(j, i)] * GaussianFilter[i][j]).rgb;
        }
    }
    
    _vOut = vOut;
}


#endif