#pragma once
#include "CEntity.h"


class CConstBuffer
    : public CEntity
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
    void UploadData(void* _pSrc, UINT _iSize = 0);
    void SetPipelineTarget(UINT8 _eSHADER_PIPELINE_FLAG) { m_ePIPELINE_STAGE_flags = _eSHADER_PIPELINE_FLAG; }
    void AddPipelineTarget(eSHADER_PIPELINE_STAGE_FLAG::FLAG _Stage) { m_ePIPELINE_STAGE_flags |= (UINT8)_Stage; }

    //기본 플래그는 MAX값으로 전달됨. 따로 지정할 경우 특정 파이프라인에만 상수버퍼가 설정된다.
    //기본값은 NONE이며, NONE 이외의 값이 전달될 경우 해당 값을 사용.
    void BindBuffer(UINT8 _eSHADER_PIPELINE_FLAG = eSHADER_PIPELINE_STAGE_FLAG::__NONE);



    CLONE_DISABLE(CConstBuffer);
public:
    CConstBuffer(UINT _iRegisterNum);
    ~CConstBuffer();
};

