#pragma once
#include "CCS_ParticleUpdate_Root.h"

#include "ptr.h"



//구조화 버퍼의 관리는 ParticleSystem에서 담당
//여기는 ParticleSystem으로부터 전달받은 구조화 버퍼를 연결된 쉐이더에서 처리해주는 역할만 담당.
//파티클시스템은 어차피 1차원 배열 형태이고 x축으로 많아봐야 1024개 스레드를 다 사용할 리도 없으므로
//x축 기준으로 연산을 해준다.

//컴퓨트쉐이더에서 INT0, INT1, INT2는 현재 사용 중

class CTexture;

class CCS_ParticleUpdate_Basic :
    public CCS_ParticleUpdate_Root
{

public:
    CCS_ParticleUpdate_Basic();
    //CCS_ParticleUpdate_Basic(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ);
    virtual ~CCS_ParticleUpdate_Basic();

public:
    virtual bool BindDataCS() override;
    virtual void UnBindCS() override;

private:
    Ptr<CTexture> m_Tex_Noise;

public:
    void SetNoiseTexture(Ptr<CTexture> _pTex) { m_Tex_Noise = _pTex; }

    void SetParticleOwnerPos(const Vec3& _vPos) { SetScalarParam(MTRL_SCALAR_OWNER_OBJ_POS, Vec4(_vPos, 1.f)); }
};



