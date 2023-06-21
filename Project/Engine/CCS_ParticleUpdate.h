#pragma once
#include "CComputeShader.h"

#include "ptr.h"

class CStructBuffer;
class CConstBuffer;
class CParticleSystem;
class CTexture;

class CCS_ParticleUpdate :
    public CComputeShader
{
private:
    CCS_ParticleUpdate();
public:
    CCS_ParticleUpdate(const string_view _strFileKey);
    CCS_ParticleUpdate(UINT _uNumThreadsX, UINT _uNumThreadsY, UINT _uNumThreadsZ);
    virtual ~CCS_ParticleUpdate();

public:
    virtual bool BindDataCS() override;
    virtual void UnBindCS() override;

private:
    //아래 버퍼들을 소유한 컴포넌트의 주소도 받아놓는다.
    CParticleSystem* m_pBufferOwner;
    CStructBuffer* m_pSBuffer_Transform;
    CStructBuffer* m_pSBufferRW_Shared;
    CConstBuffer* m_pCBuffer_ModuleData;

    Ptr<CTexture> m_Tex_Noise;

public:
    void SetNoiseTexture(Ptr<CTexture> _pTex) { m_Tex_Noise = _pTex; }

    void SetBuffers(CParticleSystem* _pBufferOwner, CStructBuffer* _pSBuffer_Transform, CStructBuffer* _pSBuffer_SharedRW, CConstBuffer* _pCBuffer_ModuleData);

    void SetParticleOwnerPos(const Vec3& _vPos) { SetMtrlScalarParam(MTRL_SCALAR_OWNER_OBJ_POS, Vec4(_vPos, 1.f)); }
};

inline void CCS_ParticleUpdate::SetBuffers(CParticleSystem* _pBufferOwner, CStructBuffer* _pSBuffer_Transform, CStructBuffer* _pSBuffer_SharedRW, CConstBuffer* _pCBuffer_ModuleData)
{

    m_pBufferOwner = _pBufferOwner;
    m_pSBuffer_Transform = _pSBuffer_Transform;
    m_pSBufferRW_Shared = _pSBuffer_SharedRW;
    m_pCBuffer_ModuleData = _pCBuffer_ModuleData;
}

