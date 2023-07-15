#pragma once
#include "cShaderDataModule.h"

#include "Ptr.h"

class cStructBuffer;
class cConstBuffer;
class cCom_Renderer_Particle;
class cTexture;

class cCSModule_ParticleBasic :
    public cShaderDataModule
{
public:
    cCSModule_ParticleBasic();
    virtual ~cCSModule_ParticleBasic();

    cCSModule_ParticleBasic(cCSModule_ParticleBasic const& _other);
    CLONE(cCSModule_ParticleBasic);

public:
    virtual tNumData BindDataCS() override;
    virtual bool BindDataGS() override;
    virtual void UnBind() override;

private:
    //아래 버퍼들을 소유한 컴포넌트의 주소도 받아놓는다.
    cCom_Renderer_Particle* m_pBufferOwner;
    cStructBuffer* m_pSBuffer_Transform;
    cStructBuffer* m_pSBufferRW_Shared;
    cConstBuffer* m_pCBuffer_ModuleData;

    Ptr<cTexture> m_Tex_Noise;

public:
    void SetNoiseTexture(Ptr<cTexture> _pTex) { m_Tex_Noise = _pTex; }
};

