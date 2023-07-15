#pragma once
#include "IRenderer.h"

#include "S_H_Particle.hlsli"


class cStructBuffer;
class cCSModule_ParticleBasic;
class cCom_Camera;
class cComputeShader;
class cShaderDataModule;

class cCom_Renderer_Particle :
    public IRenderer
{
public:
    cCom_Renderer_Particle();
    virtual ~cCom_Renderer_Particle();

    cCom_Renderer_Particle(cCom_Renderer_Particle const& _other);
    CLONE(cCom_Renderer_Particle)

public:
    virtual void init() override;
    virtual void finaltick() override;
    virtual bool render() override;
    virtual void cleanup() override {};

private:
    

    //파티클 데이터를 담아놓을 데이터 모듈. 각 파티클마다 고유 정보를 가지고 있어야 하므로 unique_ptr
    std::unique_ptr<cShaderDataModule> m_pShaderDataModule;

    //파티클 데이터를 처리해줄 컴퓨트쉐이더 주소
    Ptr<cComputeShader> m_pCS;

    //1. 데이터 관련 변수
    ////tParticele 데이터를 전달할 구조화 버퍼
    ////각 파티클별 정보가 전달될 버퍼
    //cStructBuffer* m_pSBufferRW_ParticleTransform;

    ////추가적으로 쓰기 가능한 공유 파티클 버퍼. 
    ////tParticleShareData 전달용
    //cStructBuffer* m_pSBufferRW_Shared;


    //Create 함수를 호출해서 구조화 버퍼를 만들었는지 여부를 저장할 변수
    //Create 함수를 호출하지 않았을 경우 로직 함수들을 돌지 않음
    //bool m_bIsCreated;

    ////이외에도 추가적으로 파티클 모듈을 전달할 상수버퍼가 있음.
    ////이건 메소드 내부에서 static 변수로 사용할 예정
    //tParticleModule m_tModuleData;
    //float m_AccTime;
    
    //2.파티클 처리용 컴퓨트쉐이더 주소
   // Ptr<cComputeShader> m_pCSParticle;


    //3.렌더링용 그래픽쉐이더 주소 - 이건 부모 클래스에 정의되어있음.


    ////파티클에 사용될 텍스처. - cMaterial에 정의되어 있음.

public:
    void CreateParticle();

    //처리해줄 파티클 쉐이더를 지정
    //나중에 파티클을 여러 종류로 구현하게 되면 
    //필요한 파티클을 키로 찾아서 사용하는 방식
    void SetParticleCS(const string_view _strKeyCS);
};

