#pragma once

#include "CEntity.h"

struct tSBufferDesc
{
    //자신의 공유정보를 담고있는 상수버퍼 내부에서의 인덱스
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
    //아무 인자 없이 생성할 경우 반드시 SetInfo를 해 줄것.
    CStructBuffer();
    CStructBuffer(const tSBufferDesc& _tDesc);
    CStructBuffer(tSBufferDesc&& _tDesc);
    virtual ~CStructBuffer();
    CLONE_DISABLE(CStructBuffer)

private:
    tSBufferDesc                m_tSBufferDesc;
    bool                        m_bSBufferDescSet;
   
    UINT                        m_uElementStride;   //구조체 하나 당 바이트 갯수
    UINT                        m_uElementCount;    //현재 등록한 구조체의 갯수
    UINT                        m_uElementCapacity; //현재 확보되어있는 구조체의 갯수


    D3D11_BUFFER_DESC           m_BufferDesc;
    ComPtr<ID3D11Buffer>        m_StructBuffer;

    ComPtr<ID3D11ShaderResourceView> m_SRV;

    //RW 형태로 바인딩하고자 할때
    ComPtr<ID3D11UnorderedAccessView> m_UAV;

    ComPtr<ID3D11Buffer>    m_StagingBuffer;
    
    eCURRENT_BOUND_VIEW     m_eCurBoundView;

    

public:
    void SetDesc(const tSBufferDesc& _tDesc);

    //Setter Getter Adder
    void SetPipelineTarget(UINT _eSHADER_PIPELINE_FLAG) { m_tSBufferDesc.flag_eSHADER_PIPELINE_STAGE_FLAG_SRV = _eSHADER_PIPELINE_FLAG; }
    void AddPipelineTarget(eSHADER_PIPELINE_STAGE::FLAG _Stage) { m_tSBufferDesc.flag_eSHADER_PIPELINE_STAGE_FLAG_SRV |= (UINT)_Stage; }

    UINT GetCapacity() const { return m_uElementCapacity; }

    //글로벌 변수에 있는거 리턴해주면 될듯
    UINT GetElemCount() const { return m_uElementCount; }

public:
    //처음 생성할 때 반드시 목표 파이프라인 타겟과 레지스터 번호를 설정해줄 것
    void Create(UINT _uiElementStride, UINT _uElementCapacity, void* _pInitialData, UINT _uElemCount);

    //데이터를 버퍼로 전송
    void UploadData(void* _pData, UINT _uCount);
    
    //데이터를 받아옴
    void GetData(void* _pDest, UINT _uDestByteCapacity);

    //데이터를 특정 레지스터에 바인딩. SRV에 바인딩할것인지 UAV에 바인딩할것인지를 지정
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