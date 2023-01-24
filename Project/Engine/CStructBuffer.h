#pragma once

#include "CEntity.h"

class CStructBuffer
    : public CEntity
{
public:
    CStructBuffer();
    virtual ~CStructBuffer();
    CLONE_DISABLE(CStructBuffer);

private:
    UINT                    m_uiElementSize;
    UINT                    m_uiElementCount;

    UINT8                   m_ePIPELINE_STAGE_flags;
    UINT                    m_uiRegisterIdx;

    D3D11_BUFFER_DESC       m_BufferDesc;
    ComPtr<ID3D11Buffer>    m_StructBuffer;
    ComPtr<ID3D11ShaderResourceView> m_SRV;

    ComPtr<ID3D11Buffer>                m_TempDeleteSB;
    ComPtr<ID3D11ShaderResourceView>    m_TempDeleteSRV;


public:
    //Setter Adder
    void SetPipelineTarget(UINT8 _ePIPELINE_STAGE_flags) { m_ePIPELINE_STAGE_flags = _ePIPELINE_STAGE_flags; }
    void AddPipelineTarget(eSHADER_PIPELINE_STAGE_FLAG _Stage) { m_ePIPELINE_STAGE_flags |= (UINT8)_Stage; }
    void SetRegisterIdx(UINT _uiRegisterIdx) { m_uiRegisterIdx = _uiRegisterIdx; }

public:
    //처음 생성할 때 반드시 목표 파이프라인 타겟과 레지스터 번호를 설정해줄 것
    void Create(UINT _uiElementSize, UINT _uiElementCount);
    void SetData(void* _pData, UINT _uiCount);
    void UpdateData();
};

