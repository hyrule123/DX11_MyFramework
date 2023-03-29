#pragma once
#include "CEntity.h"

#include "global.h"

class CConstBuffer
    : public CEntity
{
private:
    ComPtr<ID3D11Buffer>    m_CB;
    D3D11_BUFFER_DESC       m_Desc;
    const UINT              m_iRegisterNum;

    UINT                    m_iElementSize;
    UINT                    m_iElementCount;

    UINT                   m_ePIPELINE_STAGE_flags;
        

public:
    void Create(UINT _iElementSize, UINT _iElementCount);
    void UploadData(const void* _pSrc, UINT _iSize = 0);
    void SetPipelineTarget(UINT _eSHADER_PIPELINE_FLAG) { m_ePIPELINE_STAGE_flags = _eSHADER_PIPELINE_FLAG; }
    void AddPipelineTarget(eSHADER_PIPELINE_STAGE::FLAG _Stage) { m_ePIPELINE_STAGE_flags |= (UINT)_Stage; }

    //기본 플래그는 MAX값으로 전달됨. 따로 지정할 경우 특정 파이프라인에만 상수버퍼가 설정된다.
    //기본값은 NONE이며, NONE 이외의 값이 전달될 경우 해당 값을 사용.
    void BindBuffer(UINT _eSHADER_PIPELINE_FLAG = eSHADER_PIPELINE_STAGE::__NONE);



    CLONE_DISABLE(CConstBuffer);
public:
    CConstBuffer(UINT _iRegisterNum);
    ~CConstBuffer();
};

