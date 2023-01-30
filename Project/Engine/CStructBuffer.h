#pragma once

#include "CEntity.h"

class CStructBuffer
    : public CEntity
{
public:
    CStructBuffer() = delete;
    CStructBuffer(eSBUFFER_SHARED_CBUFFER_IDX _idx);
    virtual ~CStructBuffer();
    CLONE_DISABLE(CStructBuffer);

private:
    //�ڽ��� ���������� ����ִ� ������� ���ο����� �ε���
    const eSBUFFER_SHARED_CBUFFER_IDX m_eCBufferIdx;

    UINT                    m_uElementSize;
    UINT                    m_uElementCapacity;

    UINT8                   m_flagPipelineTarget;
    UINT                    m_uRegisterIdx;

    D3D11_BUFFER_DESC       m_BufferDesc;
    ComPtr<ID3D11Buffer>    m_StructBuffer;
    ComPtr<ID3D11ShaderResourceView> m_SRV;

    ComPtr<ID3D11Buffer>                m_TempDeleteSB;
    ComPtr<ID3D11ShaderResourceView>    m_TempDeleteSRV;


public:
    //Setter Adder
    void SetPipelineTarget(UINT8 _eSHADER_PIPELINE_FLAG) { m_flagPipelineTarget = _eSHADER_PIPELINE_FLAG; }
    void AddPipelineTarget(eSHADER_PIPELINE_STAGE_FLAG _Stage) { m_flagPipelineTarget |= (UINT8)_Stage; }
    void SetRegisterIdx(UINT _uiRegisterIdx) { m_uRegisterIdx = _uiRegisterIdx; }

    UINT GetCapacity() const { return m_uElementCapacity; }
    

public:
    //ó�� ������ �� �ݵ�� ��ǥ ���������� Ÿ�ٰ� �������� ��ȣ�� �������� ��
    void Create(UINT _uiElementSize, UINT _uElementCapacity);
    void SetData(void* _pData, UINT _uiCount);
    void UpdateData();
};

