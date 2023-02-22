#include "pch.h"
#include "CAnimator2D.h"

#include "CAnim2DAtlas.h"

#include "CResMgr.h"

#include "CMeshRender.h"
#include "CMaterial.h"

#include "CTimeMgr.h"

CAnimator2D::CAnimator2D()
    : CComponent(eCOMPONENT_TYPE::ANIMATOR2D)
    , m_uCurFrame()
    , m_fCurTime()
    , m_bFinish()
    , m_fTimePerFrame()
    , m_fFullPlayTime()
    , m_eLoopMode()
    , m_bReverse()
    , m_pCurAnim()
    , m_uMaxFrameCount()
{
}

CAnimator2D::~CAnimator2D()
{
}

void CAnimator2D::tick()
{
    //재생할 애니메이션이 없거나 애니메이션 재생 종료시 return
    if (nullptr == m_pCurAnim || true == m_bFinish)
        return;

    //시간 추가
    m_fCurTime += DELTA_TIME;


    //프레임당 재생시간을 넘어갔을 경우
    if (m_fTimePerFrame < m_fCurTime)
    {
        //ftime에서 한프레임별 시간을 뺴준후
        m_fCurTime -= m_fTimePerFrame;

        //다음 프레임으로 진행한다.
        ++m_uCurFrame;

        //마지막 프레임일 경우
        if (m_uMaxFrameCount <= m_uCurFrame)
        {
            //마지막 프레임으로 고정시킨다
            m_uCurFrame = m_uMaxFrameCount - 1;
            m_bFinish = true;
        }
    }

    //재생이 방금 끝났는데
    if (true == m_bFinish)
    {
        //역재생이나 루프가 설정되어있을 경우
        switch (m_eLoopMode)
        {
        case eANIM_LOOPMODE::NONE:
            //해줄거 없음
            break;
        case eANIM_LOOPMODE::NORMAL_LOOP:
            //프레임을 다시 0번으로 변경
            m_uCurFrame = 0u;

            m_bFinish = false;
            break;
        case eANIM_LOOPMODE::ZIG_ZAG:
            m_uCurFrame = 0u;

            //프레임 0번으로 변경하고 리버스 모드를 반대로 바꿈
            m_bReverse = !m_bReverse;

            m_bFinish = false;
            break;
        default:
            break;
        }
    }


    //역재생인지 여부를 먼저 계산
    if (true == m_bReverse)
        m_uCurFrameIdx = m_uMaxFrameCount - 1u - m_uCurFrame;
    else
        m_uCurFrameIdx = m_uCurFrame;

    //나중에 여기서 방향성을 가지면 어떡할것인지 등등을 추가할것
    m_uCalculatedIdx = m_pCurAnim->vecFrame[m_uCurFrameIdx].uIdx;
    
    //프레임에 등록된 콜백함수가 있을 경우 콜백함수 호출
    size_t size = m_pCurAnim->vecFrame[m_uCurFrameIdx].pfuncCallback.size();
    if ((size_t)0 != size)
    {
        for (size_t i = 0; i < size; i++)
        {
            //콜백함수를 호출
            m_pCurAnim->vecFrame[m_uCurFrameIdx].pfuncCallback[i]();
        }
    }
}

void CAnimator2D::finaltick()
{
    //재생할 애니메이션이 없을 시 
    if (nullptr == m_pCurAnim)
    {
        m_uCalculatedIdx = -1;
        return;
    }
        

    UpdateData();
}

void CAnimator2D::UpdateData()
{
    //애니메이션의 UV 정보를 받아옴.
    const tAnimFrameUV& frameuv = m_pCurAnimSprite->GetFrameUVData(m_pCurAnim->vecFrame[m_uCurFrameIdx].uIdx);
        
    int iAnimFlag = (int)eANIM2D_FLAG::USEANIM;
    
    if (Vec2(0.f, 0.f) != m_pCurAnim->vPivot)
    {
        iAnimFlag |= (int)eANIM2D_FLAG::USEPIVOT;
    }

    CGameObject* pOwner = GetOwner();
    pOwner->SetScalarParam(MTRL_SCALAR_STD2D_FLAG, &iAnimFlag);
    pOwner->SetScalarParam(MTRL_SCALAR_STD2D_ANIM_UV_LEFTTOP, &(frameuv.LeftTopUV));
    pOwner->SetScalarParam(MTRL_SCALAR_STD2D_ANIM_UV_SLICE, &(frameuv.SliceUV));
    pOwner->SetScalarParam(MTRL_SCALAR_STD2D_ANIM_UV_OFFSET, &(frameuv.Offset));
    pOwner->SetScalarParam(MTRL_SCALAR_STD2D_PIVOT, &(m_pCurAnim->vPivot));


    pOwner->GetRenderComponent()->GetCurMaterial()->SetTexParam(eMTRLDATA_PARAM_TEX::_0, m_pCurAnimSprite->GetAtlasTex());
}

void CAnimator2D::SetAtlasTex(const string& _AtlasTexStrKey)
{
    m_pCurAnimSprite = CResMgr::GetInst()->FindRes<CAnim2DAtlas>(_AtlasTexStrKey);
}

void CAnimator2D::Play(const string& _strAnimName, eANIM_LOOPMODE _eLoopMode, bool _bReverse)
{
    if (nullptr == m_pCurAnimSprite)
        return;

    m_pCurAnim = m_pCurAnimSprite->FindAnim2D(_strAnimName);
    
    if (nullptr == m_pCurAnim)
        return;

    m_eLoopMode = _eLoopMode;
    m_bReverse = _bReverse;

    m_uCurFrame = 0;
    
    m_uMaxFrameCount = m_pCurAnim->uNumFrame;
    m_fTimePerFrame = m_pCurAnim->fTimePerFrame;
    m_fFullPlayTime = m_pCurAnim->fFullPlayTime;
}

void CAnimator2D::ResetCurAnim()
{
   m_uCurFrame = 0u;
   m_uCalculatedIdx = 0;
   m_fCurTime = 0.f;
   m_bFinish = false;
}