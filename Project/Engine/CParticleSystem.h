#pragma once
#include "CRenderComponent.h"

class CComputeShader;
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
    virtual void cleanup() override {};

private:
    //1. ������ ���� ����
    ////tParticele �����͸� ������ ����ȭ ����
    ////�� ��ƼŬ�� ������ ���޵� ����
    CStructBuffer* m_pSBuffer_ParticleInfo;

    ////�߰������� ���� ������ ���� ��ƼŬ ����. 
    ////tRWParticleBuffer ���޿�
    CStructBuffer* m_pSBuffer_SharedRW;


    //Create �Լ��� ȣ���ؼ� ����ȭ ���۸� ��������� ���θ� ������ ����
    //Create �Լ��� ȣ������ �ʾ��� ��� ���� �Լ����� ���� ����
    bool m_bIsCreated;

    ////�̿ܿ��� �߰������� ��ƼŬ ����� ������ ������۰� ����.
    ////�̰� �޼ҵ� ���ο��� static ������ ����� ����
    tParticleModule m_ParticleModuleData;
    float m_AccTime;
    



    //2.��ƼŬ ó���� ��ǻƮ���̴� �ּ�
    Ptr<CComputeShader> m_pCSParticle;


    //3.�������� �׷��Ƚ��̴� �ּ� - �̰� �θ� Ŭ������ ���ǵǾ�����.


    ////��ƼŬ�� ���� �ؽ�ó. - Material�� ���ǵǾ� ����.

    

public:
    void CreateParticle();

    //ó������ ��ƼŬ ���̴��� ����
    //���߿� ��ƼŬ�� ���� ������ �����ϰ� �Ǹ� 
    //�ʿ��� ��ƼŬ�� Ű�� ã�Ƽ� ����ϴ� ���
    void SetParticleCS(const string& _strKeyCS);

};

