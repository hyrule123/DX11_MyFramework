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
