#pragma once

#include "CEntity.h"



class CStructBuffer
    : public CEntity
{
public:
    CStructBuffer() = delete;
    CStructBuffer(eSTRUCT_BUFFER_TYPE _type, UINT8 _eSHADER_PIPELINE_STAGE_FLAG, eSBUFFER_SHARED_CBUFFER_IDX _CBIdx, eSRV_REGISTER_IDX _SRVIdx, eUAV_REGISTER_IDX _UAVIdx);
    virtual ~CStructBuffer();
    CLONE_DISABLE(CStructBuffer)

private:
    //�ڽ��� ���������� ����ִ� ������� ���ο����� �ε���
    eSTRUCT_BUFFER_TYPE         m_eSBufferType;
    UINT8                       m_flagPipelineTarget;

    eSBUFFER_SHARED_CBUFFER_IDX m_eCBufferIdx;
    eSRV_REGISTER_IDX           m_eSRVIdx;
    eUAV_REGISTER_IDX           m_eUAVIdx;

   
    UINT                    m_uElementStride;   //����ü �ϳ� �� ����Ʈ ����
    //ī��Ʈ�� �۷ι� ������ ����. g_arrStructBufferInfo[(UINT)m_eCBufferIdx].uSBufferCount;
    UINT                    m_uElementCapacity; //���� Ȯ���Ǿ��ִ� ����ü�� ����


    D3D11_BUFFER_DESC       m_BufferDesc;
    ComPtr<ID3D11Buffer>    m_StructBuffer;

    ComPtr<ID3D11ShaderResourceView> m_SRV;

    //RW ���·� ���ε��ϰ��� �Ҷ�
    ComPtr<ID3D11UnorderedAccessView> m_UAV;

    ComPtr<ID3D11Buffer>    m_StagingBuffer;
    
    eCURRENT_BOUND_VIEW     m_eCurrentBoundView;


public:
    //Setter Getter Adder
    void SetPipelineTarget(UINT8 _eSHADER_PIPELINE_FLAG) { m_flagPipelineTarget = _eSHADER_PIPELINE_FLAG; }
    void AddPipelineTarget(eSHADER_PIPELINE_STAGE::FLAG _Stage) { m_flagPipelineTarget |= (UINT8)_Stage; }

    UINT GetCapacity() const { return m_uElementCapacity; }

    //�۷ι� ������ �ִ°� �������ָ� �ɵ�
    UINT GetElemCount() const { return g_arrStructBufferInfo[(UINT)m_eCBufferIdx].uSBufferCount; }

public:
    //ó�� ������ �� �ݵ�� ��ǥ ���������� Ÿ�ٰ� �������� ��ȣ�� �������� ��
    void Create(UINT _uiElementSize, UINT _uElementCapacity, void* _pInitialData, UINT _uElemCount);

    //�����͸� ���۷� ����
    void UploadData(void* _pData, UINT _uCount);
    
    //�����͸� �޾ƿ�
    void GetData(void* _pDest, UINT _uDestCapacity);

    //�����͸� Ư�� �������Ϳ� ���ε�. SRV�� ���ε��Ұ����� UAV�� ���ε��Ұ������� ����
    void BindBufferSRV();
    void UnBindSRV();

    //Bind buffer with UAV Mode to Compute shader 
    void BindBufferUAV();
    void UnBindUAV();

private:
    void BindConstBuffer();

    void CreateStagingBuffer();
    void CreateSRV();
    void CreateUAV();
};

