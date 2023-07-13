#pragma once
#include "Entity.h"

#include "global.h"


class cConstBuffer
    : public Entity
{
    
public:
    cConstBuffer();
    virtual ~cConstBuffer();


    CLONE_DISABLE(cConstBuffer);

private:
    ComPtr<ID3D11Buffer>    m_CB;
    D3D11_BUFFER_DESC       m_Desc;

    tConstBufferClassDesc        m_CBufferClassDesc;

public:
    HRESULT Create(tConstBufferClassDesc const& _tDesc);
    void UploadData(const void* _pSrc, UINT _iSize = 0);
    void SetPipelineTarget(UINT _eSHADER_PIPELINE_FLAG) { m_CBufferClassDesc.PipelineStageBindFlag = _eSHADER_PIPELINE_FLAG; }
    void AddPipelineTarget(define_Shader::ePIPELINE_STAGE_FLAG::FLAG _Stage) { m_CBufferClassDesc.PipelineStageBindFlag |= (UINT)_Stage; }

    //기본 플래그는 MAX값으로 전달됨. 따로 지정할 경우 특정 파이프라인에만 상수버퍼가 설정된다.
    //기본값은 NONE이며, NONE 이외의 값이 전달될 경우 해당 값을 사용.
    void BindBuffer(UINT _eSHADER_PIPELINE_FLAG = define_Shader::ePIPELINE_STAGE_FLAG::__NONE);
};

