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
    float3 Padding;
};


//상수 버퍼 'SBUFFERINFO' 내부의 인덱스 번호를 지정하는 열거체
//enum class eSBUFFER_SHARED_CBUFFER_IDX : UINT
//{
//	LIGHT2D,
//	TILE,
//	END
//};
#define eSBUFFER_SHARED_CBUFFER_LIGHT2D 0u
#define eSBUFFER_SHARED_CBUFFER_TILE 1u
#define eSBUFFER_SHARED_CBUFFER_END 2u
