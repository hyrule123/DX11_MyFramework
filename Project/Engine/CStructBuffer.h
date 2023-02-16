#pragma once

#include "CEntity.h"



class CStructBuffer
    : public CEntity
{
public:
    CStructBuffer() = delete;
    CStructBuffer(eSTRUCT_BUFFER_TYPE _type, UINT _eSHADER_PIPELINE_STAGE_FLAG_SRV, eCBUFFER_SBUFFER_SHAREDATA_IDX _CBIdx, int _SRVIdx, int _UAVIdx);
    virtual ~CStructBuffer();
    CLONE_DISABLE(CStructBuffer)

private:
    //�ڽ��� ���������� ����ִ� ������� ���ο����� �ε���
    eSTRUCT_BUFFER_TYPE         m_eSBufferType;
    UINT                        m_flagPipelineTargetSRV;

    eCBUFFER_SBUFFER_SHAREDATA_IDX m_eCBufferIdx;
    int                         m_e_t_SRVIdx;
    int                         m_e_u_UAVIdx;

   
    UINT                        m_uElementStride;   //����ü �ϳ� �� ����Ʈ ����
    UINT                        m_uElementCount;    //���� ����� ����ü�� ����
    UINT                        m_uElementCapacity; //���� Ȯ���Ǿ��ִ� ����ü�� ����


    D3D11_BUFFER_DESC           m_BufferDesc;
    ComPtr<ID3D11Buffer>        m_StructBuffer;

    ComPtr<ID3D11ShaderResourceView> m_SRV;

    //RW ���·� ���ε��ϰ��� �Ҷ�
    ComPtr<ID3D11UnorderedAccessView> m_UAV;

    ComPtr<ID3D11Buffer>    m_StagingBuffer;
    
    eCURRENT_BOUND_VIEW     m_eCurBoundView;


public:
    //Setter Getter Adder
    void SetPipelineTarget(UINT _eSHADER_PIPELINE_FLAG) { m_flagPipelineTargetSRV = _eSHADER_PIPELINE_FLAG; }
    void AddPipelineTarget(eSHADER_PIPELINE_STAGE::FLAG _Stage) { m_flagPipelineTargetSRV |= (UINT)_Stage; }

    UINT GetCapacity() const { return m_uElementCapacity; }

    //�۷ι� ������ �ִ°� �������ָ� �ɵ�
    UINT GetElemCount() const { return m_uElementCount; }

public:
    //ó�� ������ �� �ݵ�� ��ǥ ���������� Ÿ�ٰ� �������� ��ȣ�� �������� ��
    void Create(UINT _uiElementStride, UINT _uElementCapacity, void* _pInitialData, UINT _uElemCount);

    //�����͸� ���۷� ����
    void UploadData(void* _pData, UINT _uCount);
    
    //�����͸� �޾ƿ�
    void GetData(void* _pDest, UINT _uDestByteCapacity);

    //�����͸� Ư�� �������Ϳ� ���ε�. SRV�� ���ε��Ұ����� UAV�� ���ε��Ұ������� ����
    void BindBufferSRV();
    void UnBindSRV();

    //Bind buffer with UAV Mode to Compute shader 
    void BindBufferUAV();
    void UnBindUAV();

private:
    void BindConstBuffer(UINT _eSHADER_PIPELINE_FLAG);

    void CreateStagingBuffer();
    void CreateSRV();
    void CreateUAV();
};

