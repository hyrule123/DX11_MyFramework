#pragma once
#include "CRenderComponent.h"

class CCS_ParticleUpdate;
class CStructBuffer;

class CParticleSystem :
    public CRenderComponent
{
public:
    CParticleSystem();
    virtual ~CParticleSystem();
    CLONE(CParticleSystem)

public:
    virtual void init() override;
    virtual void finaltick() override;
    virtual void render() override;
    virtual void cleanup() override;

private:
    Ptr<CCS_ParticleUpdate> m_pParticleCS;

public:
    //처리해줄 파티클 쉐이더를 지정
    //나중에 파티클을 여러 종류로 구현하게 되면 
    //필요한 파티클을 키로 찾아서 사용하는 방식
    void SetParticleCS(const string& _ResKey);
};

