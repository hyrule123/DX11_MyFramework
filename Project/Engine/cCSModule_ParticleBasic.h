#pragma once
#include "cCShaderModule.h"

#include "Ptr.h"

class cStructBuffer;
class cConstBuffer;
class cParticleSystem;
class cTexture;

class cCSModule_ParticleBasic :
    public cCShaderModule
{
public:
    cCSModule_ParticleBasic();
    virtual ~cCSModule_ParticleBasic();

public:
    virtual bool Init() override;
    virtual bool BindData() override;
    virtual void UnBind() override;

private:
    //아래 버퍼들을 소유한 컴포넌트의 주소도 받아놓는다.
    cParticleSystem* m_pBufferOwner;
    cStructBuffer* m_pSBuffer_Transform;
    cStructBuffer* m_pSBufferRW_Shared;
    cConstBuffer* m_pCBuffer_ModuleData;

    Ptr<cTexture> m_Tex_Noise;

public:
    void SetNoiseTexture(Ptr<cTexture> _pTex) { m_Tex_Noise = _pTex; }

    void SetParticleOwnerPos(const Vec3& _vPos) { GetOwner()->SetMtrlScalarParam(MTRL_SCALAR_OWNER_OBJ_POS, Vec4(_vPos, 1.f)); }
};

