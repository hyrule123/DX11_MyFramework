#pragma once
#include "CComponent.h"

#include "ptr.h"


class CAnim2DAtlas;
class CTexture;
struct tAnimFrameIdx;

class CAnimator2D :
    public CComponent
{
public:
    CAnimator2D();
    ~CAnimator2D();
    CLONE(CAnimator2D);

public:
    virtual void tick() override;
    virtual void finaltick() override;

    virtual void cleanup() override {};

    void UpdateData();
    

private:
    //0부터 체크되는 프레임 번호
    UINT                m_uCurFrame;

    //벡터에서 찾을 현재 프레임의 인덱스 번호
    UINT                m_uCurFrameIdx;

    //실제로 계산된 vecFrameUV 안에서의 인덱스
    int                m_uCalculatedIdx;

    float               m_fCurTime;
    bool                m_bFinish;

    UINT                m_uMaxFrameCount;
    float               m_fTimePerFrame;
    float               m_fFullPlayTime;  

    eANIM_LOOPMODE      m_eLoopMode;
    bool                m_bReverse;

    Ptr<CAnim2DAtlas>    m_pCurAnimSprite;
    const tAnimFrameIdx*  m_pCurAnim;
    //TODO : 개별 콜백함수를 사용하고자 할때는 Script와 연동되도록 해줄것

public:
    //애니메이션 데이터는 텍스처와 동일한 이름을 사용할것
    void SetAtlasTex(const string& _AtlasTexStrKey);
    void Play(const string& _strName, eANIM_LOOPMODE _eLoopMode, bool _bReverse);

    
private:
    void ResetCurAnim();
};

