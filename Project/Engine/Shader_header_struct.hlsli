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
