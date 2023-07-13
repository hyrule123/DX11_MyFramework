    #pragma once

#include "Entity.h"

#include "define.h"

#include "S_H_Register.hlsli"

//============사용법==============
//1. 클래스 생성
//2. 아래의 tSBufferDesc 작성 후 SetDesc() 함수로 제출
//3. Create() 함수



struct tSBufferDesc
{
    //자신의 공유정보를 담고있는 상수버퍼 내부에서의 인덱스
    eSTRUCT_BUFFER_TYPE eSBufferType; 

    //SRV 
    UINT flag_PipelineBindTarget_SRV;

    int REGISLOT_t_SRV;
    int REGISLOT_u_UAV;

    tSBufferDesc()
        : eSBufferType()
        , flag_PipelineBindTarget_SRV()
        , REGISLOT_t_SRV(REGISLOT_t_SRV_NONE)
        , REGISLOT_u_UAV(REGISLOT_u_UAV_NONE)
    {}
};


class cStructBuffer
    : public Entity
{
public:
    //아무 인자 없이 생성할 경우 반드시 SetDesc를 해 줄것.
    cStructBuffer();
    cStructBuffer(const tSBufferDesc& _tDesc);
    cStructBuffer(tSBufferDesc&& _tDesc);

    virtual ~cStructBuffer();
    CLONE_DISABLE(cStructBuffer)

private:
    D3D11_BUFFER_DESC           m_BufferDesc;

    tSBufferDesc           m_tSBufferClassDesc;
    bool                        m_bSBufferDescSet;
   
    UINT                        m_uElementStride;   //구조체 하나 당 바이트 갯수
    UINT                        m_uElementCount;    //현재 등록한 구조체의 갯수
    UINT                        m_uElementCapacity; //현재 확보되어있는 구조체의 갯수

    
    ComPtr<ID3D11Buffer>        m_StructBuffer;

    ComPtr<ID3D11ShaderResourceView> m_SRV;

    //RW 형태로 바인딩하고자 할때
    ComPtr<ID3D11UnorderedAccessView> m_UAV;

    ComPtr<ID3D11Buffer>    m_StagingBuffer;
    
    eCURRENT_BOUND_VIEW     m_eCurBoundView;

    

public:
    void SetDesc(const tSBufferDesc& _tDesc);

    //Setter Getter Adder
    void SetPipelineTarget(UINT _eSHADER_PIPELINE_FLAG) { m_tSBufferClassDesc.flag_PipelineBindTarget_SRV = _eSHADER_PIPELINE_FLAG; }
    void AddPipelineTarget(define_Shader::ePIPELINE_STAGE_FLAG::FLAG _Stage) { m_tSBufferClassDesc.flag_PipelineBindTarget_SRV |= (UINT)_Stage; }

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
   

    //Bind buffer with UAV Mode to Compute shader 
    void BindBufferUAV();

    void UnBind();

private:
    void SetDefaultDesc();
    void BindConstBuffer(UINT _eSHADER_PIPELINE_FLAG);

    void CreateStagingBuffer();
    void CreateSRV();
    void CreateUAV();
};


inline void cStructBuffer::SetDesc(const tSBufferDesc& _tDesc)
{
    m_tSBufferClassDesc = _tDesc; 
    m_bSBufferDescSet = true;
    SetDefaultDesc();
}
