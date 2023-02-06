#pragma once
#include "CComputeShader.h"

//구조화 버퍼의 관리는 ParticleSystem에서 담당
//여기는 ParticleSystem으로부터 전달받은 구조화 버퍼를 연결된 쉐이더에서 처리해주는 역할만 담당.
//파티클시스템은 어차피 1차원 배열 형태이고 x축으로 많아봐야 1024개 스레드를 다 사용할 리도 없으므로
//x축 기준으로 연산을 해준다.

class CStructBuffer;

class CCS_ParticleUpdate :
    public CComputeShader
{

public:
    CCS_ParticleUpdate();
    //CCS_ParticleUpdate(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ);
    virtual ~CCS_ParticleUpdate();

public:
    virtual bool BindDataCS() override;
    virtual void UnBindCS() override;

private:
    CStructBuffer* m_pParticleSBuffer;

public:
    void SetData(void* _pData, UINT _uCount);


};

