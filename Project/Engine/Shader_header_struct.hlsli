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

    float4 vLightSpotDir; //스포트라이트의 방향

    float fRadius; //점광원 또는 스포트라이트의 거리
    float fAngle;
    uint LightType;
    int padding;
};