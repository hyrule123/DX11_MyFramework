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
    //累诀且 咆胶贸 康开 林家
    Ptr<CTexture> m_pTexture;

    //Map狼 Chunk 颇老 酒贰狼 瘤屈沥焊
    //CV5 林家
    //VX4 林家
    //VF4 林家
    //VR4 林家
    //WPE 林家


};

