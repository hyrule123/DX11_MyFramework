#pragma once
#include "IRenderer.h"

#include "S_H_ParticleBasic.hlsli"

#include "cCSModule_ParticleBasic.h"

class cStructBuffer;
class cCSModule_ParticleBasic;
class cCom_Camera;
class cComputeShader;
class cShaderDataModule;

class cCom_Renderer_ParticleBasic :
    public IRenderer
{
public:
    cCom_Renderer_ParticleBasic();
    virtual ~cCom_Renderer_ParticleBasic();

    cCom_Renderer_ParticleBasic(cCom_Renderer_ParticleBasic const& _other);
    CLONE(cCom_Renderer_ParticleBasic)

public:
    virtual void Init() override;
    virtual void FinalTick() override;
    virtual eRENDER_RESULT Render() override;
    virtual void CleanUp() override {};

private:
    //파티클 데이터를 처리해줄 컴퓨트쉐이더 주소
    Ptr<cComputeShader> m_CS;
    cCSModule_ParticleBasic m_ParticleDataModule;

    bool                        m_bCreated;
    tParticleShareData          m_SharedData;
    tParticleTransform          m_TransformData;
    tParticleBasicSpawnSetting  m_BasicData;
    float                       m_AccTime;

public:
    void CreateParticle();
};

