#pragma once
#include "CComputeShader.h"

#include "ptr.h"
#include "CTexture.h"

class CCS_SCMapLoader :
    public CComputeShader
{
public:
    CCS_SCMapLoader();
    virtual ~CCS_SCMapLoader();

public:
    virtual bool BindDataCS() override;
    virtual void UnBindCS() override;

private:
    //�۾��� �ؽ�ó ���� �ּ�
    Ptr<CTexture> m_pTexture;

    //Map�� Chunk ���� �Ʒ��� ��������
    //CV5 �ּ�
    //VX4 �ּ�
    //VF4 �ּ�
    //VR4 �ּ�
    //WPE �ּ�


};

