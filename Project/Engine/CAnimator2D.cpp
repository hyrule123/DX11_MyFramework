#include "pch.h"
#include "CAnimator2D.h"

#include "CAnim2DAtlas.h"

#include "CResMgr.h"

#include "CMeshRender.h"
#include "CMaterial.h"

#include "CTimeMgr.h"

#include "CTransform.h"



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
    , m_bFlipX()
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
    , m_bFlipX(_other.m_bFlipX)
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




void CAnimator2D::finaltick()
{
    //재생할 애니메이션이 있을 경우 프레임 처리
    if (nullptr != m_pCurAnim)
    {
        //재생이 끝나지 않았을 경우 현재 프레임 업데이트
        if (false == m_bFinish)
        {
            //시간 추가
            m_fCurTime += DELTA_TIME;


            //프레임당 재생시간을 넘어갔을 경우
            if (m_fTimePerFrame < m_fCurTime)
            {
                //ftime에서 한프레임별 시간을 계산
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

            //재생이 이번 프레임에 종료되었을 경우 다음 과정을 처리
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
        }

        //실제 애니메이션 프레임을 계산한다.
        //역재생인지 여부를 먼저 계산
        if (true == m_bReverse)
            m_uCurFrameIdx = m_uMaxFrameCount - 1u - m_uCurFrame;
        else
            m_uCurFrameIdx = m_uCurFrame;


        //방향을 가진 애니메이션일 경우 방향에 따른 프레임을 계산한다.
        if (eANIM_TYPE::DIRECTIONAL_COL_HALF_FLIP == m_pCurAnim->eAnimType)
        {
            CalculateDirectionalColHalfFlipAtlas();
        }
        else if (eANIM_TYPE::SEQUENTIAL == m_pCurAnim->eAnimType)
        {
            m_uCalculatedIdx = m_pCurAnim->vecFrame[m_uCurFrameIdx];
        }

        //프레임에 등록된 콜백함수가 있을 경우 콜백함수 호출
        //vector의 사이즈를 확인하고 현재 프레임 넘어서 인덱스가 존재할 경우에만 확인(최적화)
        size_t size = m_pCurAnim->vec2D_pFuncCallback.size();
        if (size > m_uCurFrameIdx)
        {
            size = m_pCurAnim->vec2D_pFuncCallback[m_uCurFrameIdx].size();
            for (size_t i = 0; i < size; ++i)
            {
                //함수 호출
                if(nullptr != m_pCurAnim->vec2D_pFuncCallback[m_uCurFrameIdx][i])
                    m_pCurAnim->vec2D_pFuncCallback[m_uCurFrameIdx][i]();
            }
        }
    }

    //재생할 애니메이션이 없더라도 업데이트는 해준다.(재생할 애니메이션이 없다고 업데이트)
    UpdateData();
}

void CAnimator2D::UpdateData()
{
    UpdateAtlasTexToMtrl();

    //재생할 애니메이션이나 선택된 애니메이션이 없을 시 애니메이션 사용 설정을 해제하고 return
    CGameObject* pOwner = GetOwner();

    //기존의 플래그값을 받아와서 플래그를 꺼 준뒤 데이터를 다시 설정해준다.
    int iAnimFlag = pOwner->GetMtrlScalarParam_Int(MTRL_SCALAR_STD2D_FLAG);
    if (nullptr == m_pCurAnim)
    {
        iAnimFlag &= ~((int)eMTRL_SCALAR_STD2D_FLAG::USE_ANIM);
        iAnimFlag &= ~((int)eMTRL_SCALAR_STD2D_FLAG::USE_PIVOT);

        pOwner->SetMtrlScalarParam(MTRL_SCALAR_STD2D_FLAG, &iAnimFlag);
        pOwner->SetMtrlScalarParam(MTRL_SCALAR_STD2D_ANIM_TEXATLAS_IDX, &m_iCurAtlasTexIdx);
        return;
    }

    //플래그값 추가 및 설정
    iAnimFlag |= (int)eMTRL_SCALAR_STD2D_FLAG::USE_ANIM;

    if (Vec2(0.5f, 0.5f) != m_pCurAnim->vPivot)
        iAnimFlag |= (int)eMTRL_SCALAR_STD2D_FLAG::USE_PIVOT;
    else
        iAnimFlag &= ~(int)eMTRL_SCALAR_STD2D_FLAG::USE_PIVOT;

    if (true == m_bFlipX)
        iAnimFlag |= (int)eMTRL_SCALAR_STD2D_FLAG::NEED_FLIP_X;
    else
        iAnimFlag &= ~(int)eMTRL_SCALAR_STD2D_FLAG::NEED_FLIP_X;
    

    pOwner->SetMtrlScalarParam(MTRL_SCALAR_STD2D_FLAG, &iAnimFlag);
    pOwner->SetMtrlScalarParam(MTRL_SCALAR_STD2D_ANIM_TEXATLAS_IDX, &m_iCurAtlasTexIdx);
 

    //애니메이션의 UV 정보를 받아옴.
    const tAnimFrameUV& frameuv = m_arrAtlasTex[m_iCurAtlasTexIdx]->GetFrameUVData(m_uCalculatedIdx);

    pOwner->SetMtrlScalarParam(MTRL_SCALAR_STD2D_ANIM_UV_LEFTTOP, &(frameuv.LeftTopUV));
    pOwner->SetMtrlScalarParam(MTRL_SCALAR_STD2D_ANIM_UV_SLICE, &(frameuv.SliceUV));
    pOwner->SetMtrlScalarParam(MTRL_SCALAR_STD2D_ANIM_UV_OFFSET, &(frameuv.Offset));
    pOwner->SetMtrlScalarParam(MTRL_SCALAR_STD2D_PIVOT, &(m_pCurAnim->vPivot));
}


void CAnimator2D::AddAtlasTex(eMTRLDATA_PARAM_TEX _eTexParam, Ptr<CAnim2DAtlas> _pAtlasTex)
{
    m_arrAtlasTex[(int)_eTexParam] = _pAtlasTex;

    m_bNeedUpdateMtrl = true;
}


void CAnimator2D::Play(const string& _strAnimName, eANIM_LOOPMODE _eLoopMode, bool _bReverse)
{
    if (nullptr != m_arrAtlasTex[m_iCurAtlasTexIdx])
    {
        const tAnim2D* curanim = m_arrAtlasTex[m_iCurAtlasTexIdx]->FindAnim2D(_strAnimName);

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
        m_bFlipX = false;

        m_uMaxFrameCount = m_pCurAnim->uNumFrame;
        m_fTimePerFrame = m_pCurAnim->fTimePerFrame;
        m_fFullPlayTime = m_pCurAnim->fFullPlayTime;
    }

}

const Vec2 CAnimator2D::GetCurFrameSize() 
{
    Vec2 size;
    if (m_pCurAnim && m_arrAtlasTex[0].Get())
    {
        const Vec2& TexSize = m_arrAtlasTex[0]->GetAtlasTex()->GetSize();
        const Vec2& SliceSize = m_arrAtlasTex[0]->GetFrameUVData(m_pCurAnim->vecFrame[0]).SliceUV;
        size = TexSize * SliceSize;
    }
    return size;
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

void CAnimator2D::CalculateDirectionalColHalfFlipAtlas()
{
    //방향의 갯수를 구한다. 12시 방향과 6시 방향은 flip할 시 중복되므로 2를 弧娩?
    int NumDirection = m_pCurAnim->uColTotal * 2 - 2;
    
    ////라디안 기준 한바퀴(2PI)에서 방향의 갯수만큼 나눠준다. 한 방향당 각도이다.
    float fAngleSlice = XM_2PI / (float)NumDirection;

    //현재 z축 회전 각도를 받아와서 360도(2PI)로 나눈 나머지를 구해준다.
    //12시 기준이고, 음수는 11시 방향 360도, 양수는 1시 방향 360도를 의미한다.
    float angle = fmodf(-Transform()->GetRelativeRot().z, XM_2PI);

    //각도가 음수일 경우(반시계방향 회전일 경우) 2PI에서 빼서(angle이 음수이므로 더해주면 됨) 양수 기준으로 바꿔준다.
    //이러면 0 ~ 360도 범위가 0 ~ 2PI 범위로 바뀌게 된다.
    if (angle < 0.f)
        angle = XM_2PI + angle;
    
    //구해진 angle에서 방향의 갯수만큼 나눠주면, 인덱스 번호의 float값이 나오게 된다.
    angle /= fAngleSlice;

    //fabsf()를 해준 값을 반올림해준다. 그러면 12시 방향이 보정된다.
    angle = roundf(fabsf(angle));

    //angle에 현재 방향의 갯수(uColTotal)을 곱해 주면 아틀라스에 따른 방향을 구할 수 있다

    //이 값을 정수로 바꾼다.
    UINT idx = (UINT)angle;

    //현재 인덱스가 반을 넘어갈 경우 반을 빼주고 flip 상태를 true로 변경한다.

    if (idx >= m_pCurAnim->uColTotal)
    {
        idx = NumDirection - idx;

        //12시 방향과 6시 방향 스프라이트는 플립하지 않음.
        //if (idx == m_pCurAnim->uColTotal - 1u || idx == 0)
        //    m_bFlipX = false;
        //else
            m_bFlipX = true;
    }
    else
    {
        m_bFlipX = false;
    }

    m_uCalculatedIdx = m_pCurAnim->vecFrame[idx * m_pCurAnim->uRowTotal + m_uCurFrameIdx];
}
