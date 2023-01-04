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

    UINT8                   m_ePIPELINE_STAGE_flags;
        

public:
    void Create(UINT _iElementSize, UINT _iElementCount);
    void SetData(void* _pSrc, UINT _iSize = 0);
    void SetPipelineTarget(UINT8 _ePIPELINE_STAGE_flags) { m_ePIPELINE_STAGE_flags = _ePIPELINE_STAGE_flags; }
    void AddPipelineTarget(eSHADER_PIPELINE_STAGE_FLAG _Stage) { m_ePIPELINE_STAGE_flags |= (UINT8)_Stage; }

    //�⺻ �÷��״� MAX������ ���޵�. ���� ������ ��� Ư�� ���������ο��� ������۰� �����ȴ�.
    void UpdateData();



    CLONE_DISABLE(CConstBuffer);
public:
    CConstBuffer(UINT _iRegisterNum);
    ~CConstBuffer();
};

