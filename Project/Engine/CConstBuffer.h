#pragma once
#include "CEntity.h"


class CConstBuffer :
    public CEntity
{
private:
    ComPtr<ID3D11Buffer>    m_CB;
    D3D11_BUFFER_DESC       m_Desc;
    const UINT              m_iRegisterNum;

    UINT                    m_iElementSize;
    UINT                    m_iElementCount;

    //상수버퍼를 업데이트할 타겟 쉐이더를 지정한다.
    //ePIPELINE_STAGE 사용할 것
    UINT8                   m_flagTargetShader;
        

public:
    void Create(UINT _iElementSize, UINT _iElementCount);
    void SetData(void* _pSrc, UINT _iSize = 0);
    void UpdateData();



    CLONE_DISABLE(CConstBuffer);
public:
    CConstBuffer(UINT _iRegisterNum);
    ~CConstBuffer();
};

