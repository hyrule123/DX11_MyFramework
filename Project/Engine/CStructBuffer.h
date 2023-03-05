#pragma once

#include "CEntity.h"

struct tSBufferDesc
{
    //�ڽ��� ���������� ����ִ� ������� ���ο����� �ε���
    eSTRUCT_BUFFER_TYPE eSBufferType; 
    UINT flag_eSHADER_PIPELINE_STAGE_FLAG_SRV;
    eCBUFFER_SBUFFER_SHAREDATA_IDX eCBufferIdx;
    int i_e_t_SRVIdx;
    int i_e_u_UAVIdx;

    //tSBufferDesc()
    //    : eSBufferType()
    //    , flag_eSHADER_PIPELINE_STAGE_FLAG_SRV()
    //    , eCBufferIdx(eCBUFFER_SBUFFER_SHAREDATA_IDX::NONE)
    //    , i_e_t_SRVIdx(e_t_SRV_NONE)
    //    , i_e_u_UAVIdx(e_u_UAV_NONE)
    //{}
};


class CStructBuffer
    : public CEntity
{
public:
    //�ƹ� ���� ���� ������ ��� �ݵ�� SetInfo�� �� �ٰ�.
    CStructBuffer();
    CStructBuffer(const tSBufferDesc& _tDesc);
    CStructBuffer(tSBufferDesc&& _tDesc);
    virtual ~CStructBuffer();
    CLONE_DISABLE(CStructBuffer)

private:
    tSBufferDesc                m_tSBufferDesc;
    bool                        m_bSBufferDescSet;
   
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
    void SetDesc(const tSBufferDesc& _tDesc);

    //Setter Getter Adder
    void SetPipelineTarget(UINT _eSHADER_PIPELINE_FLAG) { m_tSBufferDesc.flag_eSHADER_PIPELINE_STAGE_FLAG_SRV = _eSHADER_PIPELINE_FLAG; }
    void AddPipelineTarget(eSHADER_PIPELINE_STAGE::FLAG _Stage) { m_tSBufferDesc.flag_eSHADER_PIPELINE_STAGE_FLAG_SRV |= (UINT)_Stage; }

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
    void SetDefaultDesc();
    void BindConstBuffer(UINT _eSHADER_PIPELINE_FLAG);

    void CreateStagingBuffer();
    void CreateSRV();
    void CreateUAV();
};


inline void CStructBuffer::SetDesc(const tSBufferDesc& _tDesc)
{
    m_tSBufferDesc = _tDesc; 
    m_bSBufferDescSet = true;
    SetDefaultDesc();
}