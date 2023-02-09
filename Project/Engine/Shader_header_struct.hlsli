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

    float4 vLightDir; //����Ʈ����Ʈ�� ����

    float fRadius; //������ �Ǵ� ����Ʈ����Ʈ�� �Ÿ�
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
    int SpawnCount; // ���� ��ų ��ƼŬ ����
    float3 padding;
};

// Particle
struct tParticle
{
    float4 vWorldPos; // ��ƼŬ ��ġ
    float4 vWorldScale; // ��ƼŬ ũ��
    float4 vColor; // ��ƼŬ ����
    float4 vVelocity; // ��ƼŬ ���� �ӵ�
    float4 vForce; // ��ƼŬ�� �־��� ��

    float Age; // ���� �ð�
    float NomalizedAge; // ������ �����ð��� 0~1�� ����ȭ �� ��
    float LifeTime; // ����
    float Mass; // ����
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