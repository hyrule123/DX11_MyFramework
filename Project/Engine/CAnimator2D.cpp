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
    , m_uCurFrameIdx()
    , m_fCurTime()
    , m_bFinish()
    , m_fTimePerFrame()
    , m_fFullPlayTime()
    , m_eLoopMode()
    , m_bReverse()
    , m_iCurAtlasTexIdx()
    , m_pCurAnim()
    , m_uMaxFrameCount()
    , m_bNeedUpdateMtrl(true)
    
{
}

CAnimator2D::CAnimator2D(const CAnimator2D& _other)
    : CComponent(_other)
    , m_uCurFrame(_other.m_uCurFrame)
    , m_uCalculatedIdx(_other.m_uCalculatedIdx)
    , m_fCurTime(_other.m_fCurTime)
    , m_bFinish(_other.m_bFinish)
    , m_uMaxFrameCount(_other.m_uMaxFrameCount)
    , m_fTimePerFrame(_other.m_fTimePerFrame)
    , m_fFullPlayTime(_other.m_fFullPlayTime)
    , m_eLoopMode(_other.m_eLoopMode)
    , m_bReverse(_other.m_bReverse)
    , m_iCurAtlasTexIdx(_other.m_iCurAtlasTexIdx)
    , m_pCurAnim(_other.m_pCurAnim)
    , m_bNeedUpdateMtrl(false)

{
    for (int i = 0; i < (int)eMTRLDATA_PARAM_TEX::_END; ++i)
    {
        m_arrAtlasTex[i] = _other.m_arrAtlasTex[i];
    }
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
    UpdateData();
}

void CAnimator2D::UpdateData()
{
    UpdateAtlasTexToMtrl();

    //재생할 애니메이션이나 선택된 애니메이션이 없을 시 애니메이션 사용 설정을 해제하고 return
    CGameObject* pOwner = GetOwner();

    //기존의 플래그값을 받아옴
    int iAnimFlag = pOwner->GetMtrlScalarData_Int(MTRL_SCALAR_STD2D_ANIM_TEXATLAS_IDX);
    if (nullptr == m_pCurAnim)
    {
        iAnimFlag &= ~((int)eANIM2D_FLAG::USEANIM);
        iAnimFlag &= ~((int)eANIM2D_FLAG::USEPIVOT);

        pOwner->SetScalarParam(MTRL_SCALAR_STD2D_FLAG, &iAnimFlag);
        pOwner->SetScalarParam(MTRL_SCALAR_STD2D_ANIM_TEXATLAS_IDX, &m_iCurAtlasTexIdx);
        return;
    }


    //플래그값 추가 및 설정
    iAnimFlag |= (int)eANIM2D_FLAG::USEANIM;
    if (Vec2(0.f, 0.f) != m_pCurAnim->vPivot)
    {
        iAnimFlag |= (int)eANIM2D_FLAG::USEPIVOT;
    }


    pOwner->SetScalarParam(MTRL_SCALAR_STD2D_FLAG, &iAnimFlag);
    pOwner->SetScalarParam(MTRL_SCALAR_STD2D_ANIM_TEXATLAS_IDX, &m_iCurAtlasTexIdx);
 

    //애니메이션의 UV 정보를 받아옴.
    const tAnimFrameUV& frameuv = m_arrAtlasTex[m_iCurAtlasTexIdx]->GetFrameUVData(m_pCurAnim->vecFrame[m_uCurFrameIdx].uIdx);

    pOwner->SetScalarParam(MTRL_SCALAR_STD2D_ANIM_UV_LEFTTOP, &(frameuv.LeftTopUV));
    pOwner->SetScalarParam(MTRL_SCALAR_STD2D_ANIM_UV_SLICE, &(frameuv.SliceUV));
    pOwner->SetScalarParam(MTRL_SCALAR_STD2D_ANIM_UV_OFFSET, &(frameuv.Offset));
    pOwner->SetScalarParam(MTRL_SCALAR_STD2D_PIVOT, &(m_pCurAnim->vPivot));
}


void CAnimator2D::AddAtlasTex(eMTRLDATA_PARAM_TEX _eTexParam, Ptr<CAnim2DAtlas> _pAtlasTex)
{
    m_arrAtlasTex[m_iCurAtlasTexIdx] = _pAtlasTex;

    m_bNeedUpdateMtrl = true;
}


void CAnimator2D::Play(const string& _strAnimName, eANIM_LOOPMODE _eLoopMode, bool _bReverse)
{
    if (nullptr != m_arrAtlasTex[m_iCurAtlasTexIdx])
    {
        const tAnimFrameIdx* curanim = m_arrAtlasTex[m_iCurAtlasTexIdx]->FindAnim2D(_strAnimName);

        //같은 애니메이션일 경우 바꾸지 않음
        if (curanim == m_pCurAnim)
            return;

        m_pCurAnim = curanim;
    }

    //위에서 못찾았을 경우 전체 순회돌면서 찾아봄
    if (nullptr == m_pCurAnim)
    {
        for (int i = 0; i < (int)eMTRLDATA_PARAM_TEX::_END; ++i)
        {
            if (nullptr != m_arrAtlasTex[i])
            {
                m_pCurAnim = m_arrAtlasTex[i]->FindAnim2D(_strAnimName);
                if (nullptr != m_pCurAnim)
                {
                    m_iCurAtlasTexIdx = i;
                    break;
                }
                    
            }
        }
    }

    //재생 준비
    if (nullptr != m_pCurAnim)
    {
        m_eLoopMode = _eLoopMode;
        m_bReverse = _bReverse;

        m_uCurFrame = 0u;
        m_uCalculatedIdx = 0;
        m_fCurTime = 0.f;
        m_bFinish = false;

        m_uMaxFrameCount = m_pCurAnim->uNumFrame;
        m_fTimePerFrame = m_pCurAnim->fTimePerFrame;
        m_fFullPlayTime = m_pCurAnim->fFullPlayTime;
    }

}





void CAnimator2D::UpdateAtlasTexToMtrl()
{
    if (false == m_bNeedUpdateMtrl)
        return;

    CMaterial* pMtrl = GetOwner()->GetRenderComponent()->GetCurMaterial().Get();

    for (int i = 0; i < (int)eMTRLDATA_PARAM_TEX::_END; ++i)
    {
        Ptr<CTexture> pTex = nullptr;
        if (nullptr != m_arrAtlasTex[i])
            pTex = m_arrAtlasTex[i]->GetAtlasTex();

        pMtrl->SetTexParam((eMTRLDATA_PARAM_TEX)i, pTex);
    }

    m_bNeedUpdateMtrl = false;
}
