#pragma once
#include "CComputeShader.h"

class CStructBuffer;
class CConstBuffer;
class CParticleSystem;

class CCS_ParticleUpdate_Root :
    public CComputeShader
{
public:
    CCS_ParticleUpdate_Root(UINT _uNumThreadsX, UINT _uNumThreadsY, UINT _uNumThreadsZ);
    virtual ~CCS_ParticleUpdate_Root();

protected:

    //�Ʒ� ���۵��� ������ ������Ʈ�� �ּҵ� �޾Ƴ��´�.
    CParticleSystem* m_pBufferOwner;
    CStructBuffer* m_pSBuffer_Transform;
    CStructBuffer* m_pSBufferRW_Shared;
    CConstBuffer* m_pCBuffer_ModuleData;

public:
    void SetBuffers(CParticleSystem* _pBufferOwner, CStructBuffer* _pSBuffer_Transform, CStructBuffer* _pSBuffer_SharedRW, CConstBuffer* _pCBuffer_ModuleData);
};

inline void CCS_ParticleUpdate_Root::SetBuffers(CParticleSystem* _pBufferOwner, CStructBuffer* _pSBuffer_Transform, CStructBuffer* _pSBuffer_SharedRW, CConstBuffer* _pCBuffer_ModuleData)
{

    m_pBufferOwner = _pBufferOwner;
    m_pSBuffer_Transform = _pSBuffer_Transform;
    m_pSBufferRW_Shared = _pSBuffer_SharedRW;
    m_pCBuffer_ModuleData = _pCBuffer_ModuleData;
}

