#pragma once
#include "Component.h"

#include "Ptr.h"



class Anim2DAtlas;
class Texture;
struct tAnim2D;

//애니메이션이 예약한 텍스처 인덱스 : 0 ~ 3(4장)
//나머지는 일반 텍스처 예약
#define MAXNUM_ANIM 4

class Animator2D 
    : public Component
{
public:
    Animator2D();
    virtual ~Animator2D();

    Animator2D(const Animator2D& _other);
    CLONE(Animator2D);
    

public:
    virtual bool SaveJson(Json::Value* _pJVal) override;
    virtual bool LoadJson(Json::Value* _pJVal) override;


public:
    

    virtual void finaltick() override;
    virtual void cleanup() override;

    void UpdateData();
    

private:
//========= 현재 애니메이션 시간 ==================
//     매 tick마다 갱신되는 변수
    //0부터 체크되는 프레임 번호
    //UINT                m_uCurFrame;

    //벡터에서 찾을 현재 프레임의 인덱스 번호(역재생 여부에 따라 계산된 실제 인덱스)
    int                m_iCurFrameIdx;

    //실제로 계산된 vecFrameUV 안에서의 인덱스
    int                m_uCalculatedIdx;

    float               m_fCurTime;
    bool                m_bFinish;

    //루프 모드일 때 bFinish에 값이 저장되지 않음 
    //-> 다른 스크립트에서 찾을 수 있도록 한 프레임동안 지속되는 변수가 별도로 필요함
    bool                m_bFinishChecked;
//========================================================


//============ 재생중인 애니메이션의 데이터 ================
    //애니메이션 전환 시 바뀌는 변수
    int                     m_iCurAtlasTexIdx;
    const tAnim2D*          m_pCurAnim;
    UINT                    m_uMaxFrameCount;
    float                   m_fTimePerFrame;
    float                   m_fFullPlayTime;

    eANIM_PLAYMODE          m_ePlayMode;
    bool                    m_bReverse;
    bool                    m_bFlipX;
//========================================================


//=================== 등록된 아틀라스 텍스처 주소 관련 ======================
    //한 게임오브젝트가 들고있을 수 있는 아틀라스 텍스처를 4개로 제한
    Ptr<Anim2DAtlas>       m_arrAtlasTex[MAXNUM_ANIM];
    bool                    m_bNeedUpdateMtrl;
//=========================================================================

    //TODO : 개별 콜백함수를 사용하고자 할때는 Script와 연동되도록 해줄것

public:
    //애니메이션 데이터는 텍스처와 동일한 이름을 사용할것
    void AddAtlasTex(eMTRLDATA_PARAM_TEX _eTexParam, Ptr<Anim2DAtlas> _pAtlasTex);

    const tAnim2D* FindAnim(const string_view _strKey_Anim);
    bool Play(const string_view _strKey_Anim, eANIM_PLAYMODE _ePlayMode, bool _bReverse);

    //재생 '준비'만 해놓고 대기
    //PlayAgain()을 통해서 재생
    bool PreparePlay(const string_view _strKey_Anim, eANIM_PLAYMODE _ePlayMode, bool _bReverse);

    //현재 재생중인 애니메이션을 그대로 역재생함.
    void ReverseCurrent() { m_bReverse = !m_bReverse; }
    void PlayAgain();

    bool IsFinished() const { return m_bFinishChecked; }

    const Vec2  GetCurFrameSize();
    const tAnim2D* GetCurAnim() const { return m_pCurAnim; }


    
private:
    void Stop() { m_iCurAtlasTexIdx = -1; m_pCurAnim = nullptr; };
    void UpdateAtlasTexToMtrl();
    void CalculateDirectionalColHalfFlipAtlas();
};

