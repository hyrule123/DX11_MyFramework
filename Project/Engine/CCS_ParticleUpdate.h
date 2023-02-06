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
    CCS_ParticleUpdate();
    //CCS_ParticleUpdate(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ);
    virtual ~CCS_ParticleUpdate();

public:
    virtual bool BindDataCS() override;
    virtual void UnBindCS() override;

private:
    CStructBuffer* m_pParticleSBuffer;

public:
    void SetData(void* _pData, UINT _uCount);


};

