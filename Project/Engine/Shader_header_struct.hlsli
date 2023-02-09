//Header for global values or struct

struct tLightColor
{
    float4 vDiffuse;
    float4 vAmbient;
};

struct tLightInfo
{
    tLightColor LightColor;

    float4 vLightWorldPos;

    float4 vLightDir; //스포트라이트의 방향

    float fRadius; //점광원 또는 스포트라이트의 거리
    float fAngle;
    uint LightType;
    int padding;
};

struct tTile
{
    float2 vLeftTop;
    float2 vSlice;
};

struct tSBufferInfo
{
    uint g_uSBufferCount;
    int iData0;
    int iData1;
    int iData2;
};

struct tRWParticleBuffer
{
    int SpawnCount; // 스폰 시킬 파티클 개수
    float3 padding;
};

// Particle
struct tParticle
{
    float4 vWorldPos; // 파티클 위치
    float4 vWorldScale; // 파티클 크기
    float4 vColor; // 파티클 색상
    float4 vVelocity; // 파티클 현재 속도
    float4 vForce; // 파티클에 주어진 힘

    float Age; // 생존 시간
    float NomalizedAge; // 수명대비 생존시간을 0~1로 정규화 한 값
    float LifeTime; // 수명
    float Mass; // 질량
};


struct RandomResult
{
    uint4 state;
    float value;
};

uint TausStep(uint z, int S1, int S2, int S3, uint M)
{
    uint b = (((z << S1) ^ z) >> S2);
    return (((z & M) << S3) ^ b);
}

uint LCGStep(uint z, uint A, uint C)
{
    return (A * z + C);
}

RandomResult Random(uint4 state)
{
    state.x = TausStep(state.x, 13, 19, 12, 4294967294);
    state.y = TausStep(state.y, 2, 25, 4, 4294967288);
    state.z = TausStep(state.z, 3, 11, 17, 4294967280);
    state.w = LCGStep(state.w, 1664525, 1013904223);

    RandomResult result;
    result.state = state;
    result.value = 2.3283064365387e-10 * (state.x ^ state.y ^ state.z ^ state.w);

    return result;
}