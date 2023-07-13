#pragma once
#include "C_CShaderModule.h"

#include "Ptr.h"

class cStructBuffer;
class cConstBuffer;
class ParticleSystem;
class Texture;

class CCS_ParticleBasic :
    public C_ComputeShader
{
public:
    CCS_ParticleBasic();
    virtual ~CCS_ParticleBasic();

public:
    virtual bool Load(std::filesystem::path const& _FilePath) override;

private:
    //아래 버퍼들을 소유한 컴포넌트의 주소도 받아놓는다.
    ParticleSystem* m_pBufferOwner;
    cStructBuffer* m_pSBuffer_Transform;
    cStructBuffer* m_pSBufferRW_Shared;
    cConstBuffer* m_pCBuffer_ModuleData;

    Ptr<Texture> m_Tex_Noise;

public:
    void SetNoiseTexture(Ptr<Texture> _pTex) { m_Tex_Noise = _pTex; }

    void SetBuffers(ParticleSystem* _pBufferOwner, cStructBuffer* _pSBuffer_Transform, cStructBuffer* _pSBuffer_SharedRW, cConstBuffer* _pCBuffer_ModuleData);

    void SetParticleOwnerPos(const Vec3& _vPos) { SetMtrlScalarParam(MTRL_SCALAR_OWNER_OBJ_POS, Vec4(_vPos, 1.f)); }
};

inline void CCS_ParticleBasic::SetBuffers(ParticleSystem* _pBufferOwner, cStructBuffer* _pSBuffer_Transform, cStructBuffer* _pSBuffer_SharedRW, cConstBuffer* _pCBuffer_ModuleData)
{
    m_pBufferOwner = _pBufferOwner;
    m_pSBuffer_Transform = _pSBuffer_Transform;
    m_pSBufferRW_Shared = _pSBuffer_SharedRW;
    m_pCBuffer_ModuleData = _pCBuffer_ModuleData;
}

