#pragma once
#include "CComputeShader.h"

#include "ptr.h"

//����ȭ ������ ������ ParticleSystem���� ���
//����� ParticleSystem���κ��� ���޹��� ����ȭ ���۸� ����� ���̴����� ó�����ִ� ���Ҹ� ���.
//��ƼŬ�ý����� ������ 1���� �迭 �����̰� x������ ���ƺ��� 1024�� �����带 �� ����� ���� �����Ƿ�
//x�� �������� ������ ���ش�.

class CStructBuffer;
class CConstBuffer;

class CCS_ParticleUpdate :
    public CComputeShader
{

public:
    CCS_ParticleUpdate();
    //CCS_ParticleUpdate(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ);
    virtual ~CCS_ParticleUpdate();

public:
    virtual bool BindDataCS() override;
    virtual void UnBindCS() override;

private:
    CStructBuffer* m_pSBuffer_Transform;
    CStructBuffer* m_pSBuffer_SharedRW;
    CConstBuffer* m_pCBuffer_ModuleData;

    

public:
    //SetBuffers �� Excute ���ָ� ������ ������.
    void SetBuffers(CStructBuffer* _pSBuffer_Transform, CStructBuffer* _pSBuffer_SharedRW, CConstBuffer* _pCBuffer_ModuleData);



};

inline void CCS_ParticleUpdate::SetBuffers(CStructBuffer* _pSBuffer_Transform, CStructBuffer* _pSBuffer_SharedRW, CConstBuffer* _pCBuffer_ModuleData)
{
    m_pSBuffer_Transform = _pSBuffer_Transform;
    m_pSBuffer_SharedRW = _pSBuffer_SharedRW;
    m_pCBuffer_ModuleData = _pCBuffer_ModuleData;
}