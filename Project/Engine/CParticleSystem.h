#pragma once
#include "CRenderComponent.h"

#include "Shader_Particle_0_Header.hlsli"

class CStructBuffer;
class CCS_ParticleUpdate;

class CCamera;

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
    virtual void render(CCamera* _pCam) override;
    virtual void cleanup() override {};

private:
    //1. ������ ���� ����
    ////tParticele �����͸� ������ ����ȭ ����
    ////�� ��ƼŬ�� ������ ���޵� ����
    CStructBuffer* m_pSBufferRW_ParticleTransform;

    ////�߰������� ���� ������ ���� ��ƼŬ ����. 
    ////tParticleShareData ���޿�
    CStructBuffer* m_pSBufferRW_Shared;


    //Create �Լ��� ȣ���ؼ� ����ȭ ���۸� ��������� ���θ� ������ ����
    //Create �Լ��� ȣ������ �ʾ��� ��� ���� �Լ����� ���� ����
    bool m_bIsCreated;

    ////�̿ܿ��� �߰������� ��ƼŬ ����� ������ ������۰� ����.
    ////�̰� �޼ҵ� ���ο��� static ������ ����� ����
    tParticleModule m_tModuleData;
    float m_AccTime;
    



    //2.��ƼŬ ó���� ��ǻƮ���̴� �ּ�
    Ptr<CCS_ParticleUpdate> m_pCSParticle;


    //3.�������� �׷��Ƚ��̴� �ּ� - �̰� �θ� Ŭ������ ���ǵǾ�����.


    ////��ƼŬ�� ���� �ؽ�ó. - Material�� ���ǵǾ� ����.

    

public:
    void CreateParticle();

    //ó������ ��ƼŬ ���̴��� ����
    //���߿� ��ƼŬ�� ���� ������ �����ϰ� �Ǹ� 
    //�ʿ��� ��ƼŬ�� Ű�� ã�Ƽ� ����ϴ� ���
    void SetParticleCS(const string& _strKeyCS);

};

