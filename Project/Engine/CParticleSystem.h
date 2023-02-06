#pragma once
#include "CRenderComponent.h"

class CCS_ParticleUpdate;
class CStructBuffer;

class CParticleSystem :
    public CRenderComponent
{
public:
    CParticleSystem();
    virtual ~CParticleSystem();
    CLONE(CParticleSystem)

public:
    virtual void init() override;
    virtual void finaltick() override;
    virtual void render() override;
    virtual void cleanup() override;

private:
    Ptr<CCS_ParticleUpdate> m_pParticleCS;

public:
    //ó������ ��ƼŬ ���̴��� ����
    //���߿� ��ƼŬ�� ���� ������ �����ϰ� �Ǹ� 
    //�ʿ��� ��ƼŬ�� Ű�� ã�Ƽ� ����ϴ� ���
    void SetParticleCS(const string& _ResKey);
};

