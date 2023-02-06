#pragma once
#include "CRenderComponent.h"

class CCS_ParticleUpdate;
class CStructuredBuffer;

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
    CStructuredBuffer* m_pParticleInfoSBuffer;

    Ptr<CCS_ParticleUpdate> m_pParticleCS;

};

