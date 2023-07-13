#pragma once
#include "Light.h"

#include "S_H_STD2DLight.hlsli"

class Light2D :
    public Light
{
public:
    Light2D();
    //Light2D(const Light2D& _other);
    virtual ~Light2D();
    CLONE(Light2D);

private:
    tLightInfo  m_LightInfo;
    
public:
    void SetLightType(eLIGHT_TYPE _Type) { m_LightInfo.LightType = (UINT)_Type; }
    void SetAmbientColor(const Vec4& _Color) { m_LightInfo.LightColor.vAmbient = _Color; }
    void SetDiffuseColor(const Vec4& _Color) { m_LightInfo.LightColor.vDiffuse = _Color; }
    void SetLightDir(const Vec3& _vDir) { m_LightInfo.vLightDir = _vDir; }
    void SetLightRadius(float _fRadius) { m_LightInfo.fRadius = _fRadius; }
    void SetLightAngle(float _fAngle) { m_LightInfo.fAngle = _fAngle; }

public:
    //위치 업데이트가 필요함.
    void finaltick() override;
    void cleanup() override {};

};

