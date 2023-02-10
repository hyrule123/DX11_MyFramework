#ifndef SHADER_HEADER_STRUCT
#define SHADER_HEADER_STRUCT

//Header for global values or struct

#define INT32 int
#define UINT32 uint

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
    UINT32 LightType;
    INT32 padding;
};

struct tTile
{
    float2 vLeftTop;
    float2 vSlice;
};

struct tSBufferInfo
{
    UINT32 g_uSBufferCount;
    INT32 iData0;
    INT32 iData1;
    INT32 iData2;
};

struct tRWParticleBuffer
{
    INT32 iSpawnCount; // ���� ��ų ��ƼŬ ����
    float3 padding;
};

// Particle
struct tParticle
{
    float4 vLocalPos;
    float4 vWorldPos; // ��ƼŬ ��ġ
    float4 vWorldScale; // ��ƼŬ ũ��
    float4 vColor; // ��ƼŬ ����
    float4 vVelocity; // ��ƼŬ ���� �ӵ�
    float4 vForce; // ��ƼŬ�� �־��� ��

    float fAge; // ���� �ð�
    float fNomalizedAge; // ������ �����ð��� 0~1�� ����ȭ �� ��
    float fLifeTime; // ����
    float fMass; // ����
    
    int bActive;
    float3 Padding;
};


struct tParticleModule
{
	// ���� ���
    float4 vSpawnColor;
    float4 vSpawnScale;
    float3 vBoxShapeScale;
    float fSphereShapeRadius;
    INT32 eSpawnShapeType; // Sphere , Box
    INT32 iSpawnRate;
    INT32 bFollowing; // 0 World, 1 Local
    INT32 Padding0;

	// Color Change ���
    float4 vStartColor; // �ʱ� ����
    float4 vEndColor; // ���� ����

	// Scale Change ���
    float4 vStartScale; // �ʱ� ũ��
    float4 vEndScale; // ���� ũ��	

	// Module Check
    INT32 bSpawn;
    INT32 bColorChange;
    INT32 bScaleChange;
    INT32 Padding1;

    INT32 iMaxParticleCount;
    float3 Padding2;
};









#endif