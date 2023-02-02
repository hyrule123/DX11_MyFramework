#pragma once

#include "CEntity.h"

enum class eSTRUCT_BUFFER_TYPE
{
    READ_ONLY,  //SRV ONLY
    READ_WRITE  //SRV + UAV(Compute Shader)
};

class CStructBuffer
    : public CEntity
{
public:
    CStructBuffer() = delete;
    CStructBuffer(eSTRUCT_BUFFER_TYPE _Type, eSBUFFER_SHARED_CBUFFER_IDX _idx, eUAV_REGISTER_USAGE _Usage);
    virtual ~CStructBuffer();
    CLONE_DISABLE(CStructBuffer)

private:
    const eSTRUCT_BUFFER_TYPE m_eSBufferType;

    //자신의 공유정보를 담고있는 상수버퍼 내부에서의 인덱스
    const eSBUFFER_SHARED_CBUFFER_IDX m_eCBufferIdx;

    UINT                    m_uElementSize;
    UINT                    m_uElementCapacity;

    UINT8                   m_flagPipelineTarget;
    UINT                    m_uRegisterIdx;

    D3D11_BUFFER_DESC       m_BufferDesc;
    ComPtr<ID3D11Buffer>    m_StructBuffer;

    ComPtr<ID3D11ShaderResourceView> m_SRV;

    //RW 형태로 바인딩하고자 할때
    ComPtr<ID3D11UnorderedAccessView> m_UAV;

    ComPtr<ID3D11Buffer>    m_StagingBuffer;
    

    bool m_bUAVBind;


public:
    //Setter Adder
    void SetPipelineTarget(UINT8 _eSHADER_PIPELINE_FLAG) { m_flagPipelineTarget = _eSHADER_PIPELINE_FLAG; }
    void AddPipelineTarget(eSHADER_PIPELINE_STAGE_FLAG _Stage) { m_flagPipelineTarget |= (UINT8)_Stage; }

    void SetRegisterIdx(UINT _uiRegisterIdx) { m_uRegisterIdx = _uiRegisterIdx; }

    UINT GetCapacity() const { return m_uElementCapacity; }

public:
    //처음 생성할 때 반드시 목표 파이프라인 타겟과 레지스터 번호를 설정해줄 것
    void Create(UINT _uiElementSize, UINT _uElementCapacity, );
    void UpdateData(void* _pData, UINT _uiCount);
    void GetData(void* _pDest);
    void BindData();
    void BindData_CS(int TargetReg = -1);

private:
    void UpdateConstBuffer();

    void CreateSRV();
    void CreateUAV();
};

