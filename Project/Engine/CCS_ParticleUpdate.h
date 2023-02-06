#pragma once
#include "CComputeShader.h"

//����ȭ ������ ������ ParticleSystem���� ���
//����� ParticleSystem���κ��� ���޹��� ����ȭ ���۸� ����� ���̴����� ó�����ִ� ���Ҹ� ���.
//��ƼŬ�ý����� ������ 1���� �迭 �����̰� x������ ���ƺ��� 1024�� �����带 �� ����� ���� �����Ƿ�
//x�� �������� ������ ���ش�.

class CStructBuffer;

class CCS_ParticleUpdate :
    public CComputeShader
{

public:
    CCS_ParticleUpdate() = delete;
    CCS_ParticleUpdate(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ);
    virtual ~CCS_ParticleUpdate();

public:
    virtual bool BindDataCS() override;
    virtual void UnBindCS() override;

private:
    CStructBuffer* m_ParticleBuffer;

public:
    void SetParticleBuffer(CStructBuffer* _pBuffer);


};

