#pragma once
#include "cShaderDataModule.h"
#include "Ptr.h"

#include "S_H_ParticleBasic.hlsli"

class cStructBuffer;
class cTexture;

class cCSModule_ParticleBasic :
    public cShaderDataModule
{
    friend class cCom_Renderer_ParticleBasic;

public:
    cCSModule_ParticleBasic();
    virtual ~cCSModule_ParticleBasic();

    cCSModule_ParticleBasic(cCSModule_ParticleBasic const& _other);
    CLONE(cCSModule_ParticleBasic);

public:
    virtual tNumDataCS BindDataCS() override;
    virtual bool BindDataGS() override;
    virtual void UnBind() override;

private:
    std::unique_ptr<cStructBuffer> m_SBufferRW_Shared;
    std::unique_ptr<cStructBuffer> m_SBuffer_ParticleSpawnSetting;
    std::unique_ptr<cStructBuffer> m_SBufferRW_Transform;
    Ptr<cTexture> m_Tex_Noise;

public:
    void SetNoiseTex(Ptr<cTexture> _pTex) { m_Tex_Noise = _pTex; }
};

