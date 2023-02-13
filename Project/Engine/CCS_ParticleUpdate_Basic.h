#pragma once
#include "CCS_ParticleUpdate_Root.h"

#include "ptr.h"



//����ȭ ������ ������ ParticleSystem���� ���
//����� ParticleSystem���κ��� ���޹��� ����ȭ ���۸� ����� ���̴����� ó�����ִ� ���Ҹ� ���.
//��ƼŬ�ý����� ������ 1���� �迭 �����̰� x������ ���ƺ��� 1024�� �����带 �� ����� ���� �����Ƿ�
//x�� �������� ������ ���ش�.

//��ǻƮ���̴����� INT0, INT1, INT2�� ���� ��� ��

class CTexture;

class CCS_ParticleUpdate_Basic :
    public CCS_ParticleUpdate_Root
{

public:
    CCS_ParticleUpdate_Basic();
    //CCS_ParticleUpdate_Basic(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ);
    virtual ~CCS_ParticleUpdate_Basic();

public:
    virtual bool BindDataCS() override;
    virtual void UnBindCS() override;

private:
    Ptr<CTexture> m_Tex_Noise;

public:
    void SetNoiseTexture(Ptr<CTexture> _pTex) { m_Tex_Noise = _pTex; }

    void SetParticleOwnerPos(const Vec3& _vPos) { SetScalarParam(OWNER_OBJ_POS, Vec4(_vPos, 1.f)); }
};



