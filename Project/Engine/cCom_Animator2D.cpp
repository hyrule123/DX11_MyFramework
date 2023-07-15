#include "pch.h"
#include "cCom_Animator2D.h"

#include "cAnim2DAtlas.h"

#include "cResMgr.h"

#include "cCom_Renderer_Basic.h"
#include "cMaterial.h"

#include "cTimeMgr.h"

#include "cCom_Transform.h"

#include "jsoncpp.h"


cCom_Animator2D::cCom_Animator2D()
    : IComponent(eCOMPONENT_TYPE::ANIMATOR2D)
    //, m_uCurFrame()
    , m_iCurFrameIdx()
    , m_fCurTime()
    , m_bFinish()
    , m_bFinishChecked()
    , m_fTimePerFrame()
    , m_fFullPlayTime()
    , m_ePlayMode()
    , m_bReverse()
    , m_bFlipX()
    , m_iCurAtlasTexIdx()
    , m_pCurAnim()
    , m_uMaxFrameCount()
    , m_bNeedUpdateMtrl(true)
    , m_uCalculatedIdx()
    
    
{
}

cCom_Animator2D::cCom_Animator2D(const cCom_Animator2D& _other)
    : IComponent(_other)
    //, m_uCurFrame(_other.m_uCurFrame)
    , m_uCalculatedIdx(_other.m_uCalculatedIdx)
    , m_fCurTime(_other.m_fCurTime)
    , m_bFinish(_other.m_bFinish)
    , m_uMaxFrameCount(_other.m_uMaxFrameCount)
    , m_fTimePerFrame(_other.m_fTimePerFrame)
    , m_fFullPlayTime(_other.m_fFullPlayTime)
    , m_ePlayMode(_other.m_ePlayMode)
    , m_bReverse(_other.m_bReverse)
    , m_bFlipX(_other.m_bFlipX)
    , m_iCurAtlasTexIdx(_other.m_iCurAtlasTexIdx)
    , m_pCurAnim(_other.m_pCurAnim)
    , m_bNeedUpdateMtrl(true)

{
    for (int i = 0; i < MAXNUM_ANIM; ++i)
    {
        m_arrAtlasTex[i] = _other.m_arrAtlasTex[i];
    }
}

cCom_Animator2D::~cCom_Animator2D()
{
}


bool cCom_Animator2D::SaveJson(Json::Value* _pJVal)
{
    if (nullptr == _pJVal)
        return false;
    else if (false == IComponent::SaveJson(_pJVal))
        return false;

    Json::Value& jVal = *_pJVal;

    try
    {
        string strKey = string(RES_INFO::PREFAB::COMPONENT::ANIMATOR2D::JSON_KEY::m_arrAtlasTex);
        jVal[strKey] = Json::Value(Json::arrayValue);
        for (int i = 0; i < MAXNUM_ANIM; ++i)
        {
            if (nullptr != m_arrAtlasTex[i])
            {
                jVal[strKey].append(m_arrAtlasTex[i]->GetKey());
            }
            else
            {
                jVal[strKey].append(Json::Value(Json::nullValue));
            }
        }
    }
    catch (const std::runtime_error& _err)
    {
        string error = _err.what();
        error += " Save Failed!!";
        MessageBoxA(nullptr, error.c_str(), nullptr, MB_OK);
        return false;
    }

    return true;
}

bool cCom_Animator2D::LoadJson(Json::Value* _pJVal)
{
    if (nullptr == _pJVal)
        return false;
    else if (false == IComponent::SaveJson(_pJVal))
        return false;

    Json::Value& jVal = *_pJVal;

    try
    {
        {
            string strKey = string(RES_INFO::PREFAB::COMPONENT::ANIMATOR2D::JSON_KEY::m_arrAtlasTex);
            if (false == jVal.isMember(strKey) || false == jVal[strKey].isArray())
                throw std::runtime_error(strKey);

            Json::Value& arrAtlasTex = jVal[strKey];

            int size = arrAtlasTex.size();
            if (size < MAXNUM_ANIM)
            {
                throw(std::runtime_error("Atlas cTexture Data corrupted."));
            }

            for (int i = 0u; i < MAXNUM_ANIM; ++i)
            {
                if (arrAtlasTex[i].isNull())
                    continue;

                Ptr<cAnim2DAtlas> pAtlas = cResMgr::GetInst()->Load<cAnim2DAtlas>(arrAtlasTex[i].asString());

                if (nullptr == pAtlas)
                {
                    DEBUG_BREAK;
                    throw(std::runtime_error(strKey));
                }

                AddAtlasTex((eMTRLDATA_PARAM_TEX)i, pAtlas);
            }
        }

    }
    catch (const std::runtime_error& _err)
    {
        string error = _err.what();
        error += " Load Failed!!";
        MessageBoxA(nullptr, error.c_str(), nullptr, MB_OK);
        return false;
    }

    return true;
}


void cCom_Animator2D::finaltick()
{
    //재생할 애니메이션이 있을 경우 프레임 처리
    if (nullptr != m_pCurAnim)
    {
        //재생이 끝나지 않았을 경우 현재 프레임 업데이트
        if (false == m_bFinish)
        {
            m_bFinishChecked = false;
            //시간 추가
            m_fCurTime += DELTA_TIME;


            //프레임당 재생시간을 넘어갔을 경우
            if (m_fTimePerFrame < m_fCurTime)
            {
                //ftime에서 한프레임별 시간을 계산
                m_fCurTime -= m_fTimePerFrame;

                //정재생일 경우
                if (false == m_bReverse)
                {
                    //다음 프레임으로 진행한다.
                    ++m_iCurFrameIdx;

                    //마지막 프레임일 경우
                    if ((int)m_uMaxFrameCount <= m_iCurFrameIdx)
                    {
                        //마지막 프레임으로 고정시킨다
                        m_iCurFrameIdx = (int)m_uMaxFrameCount - 1;
                        m_bFinish = true;
                        m_bFinishChecked = true;
                    }
                }

                //역재생일 경우
                else
                {
                    //다음 프레임으로 진행한다.
                    --m_iCurFrameIdx;

                    //마지막 프레임까지 재생이 끝났을 경우
                    if (0 > m_iCurFrameIdx)
                    {
                        //마지막 프레임으로 고정시킨다
                        m_iCurFrameIdx = 0;
                        m_bFinish = true;
                        m_bFinishChecked = true;
                    }
                }
            }

            

            //재생이 이번 프레임에 종료되었을 경우 다음 과정을 처리
            if (true == m_bFinish)
            {
                //역재생이나 루프가 설정되어있을 경우
                switch (m_ePlayMode)
                {
                case eANIM_PLAYMODE::NONE:
                    //해줄거 없음
                    break;
                case eANIM_PLAYMODE::DISABLE_AFTER_PLAY:
                {
                    RenderComponent()->SetDisable(true);
                }
                    break;
                case eANIM_PLAYMODE::NORMAL_LOOP:
                    //프레임을 다시 0번으로 변경
                    if (false == m_bReverse)
                        m_iCurFrameIdx = 0;
                    else
                        m_iCurFrameIdx = (int)m_uMaxFrameCount - 1;

                    m_bFinish = false;
                    break;
                case eANIM_PLAYMODE::ZIG_ZAG:

                    //프레임 0번으로 변경하고 리버스 모드를 반대로 바꿈
                    m_bReverse = !m_bReverse;
                    m_bFinish = false;
                    break;
                default:
                    break;
                }
            }
        }


        //방향을 가진 애니메이션일 경우 방향에 따른 프레임을 계산한다.
        if (eANIM_TYPE::DIRECTIONAL_COL_HALF_FLIP == m_pCurAnim->eAnimType)
        {
            CalculateDirectionalColHalfFlipAtlas();
        }
        else if (eANIM_TYPE::SEQUENTIAL == m_pCurAnim->eAnimType)
        {
            m_uCalculatedIdx = m_pCurAnim->vecFrame[m_iCurFrameIdx];
        }

        //프레임에 등록된 콜백함수가 있을 경우 콜백함수 호출
        //vector의 사이즈를 확인하고 현재 프레임 넘어서 인덱스가 존재할 경우에만 확인(최적화)
        size_t size = m_pCurAnim->vec2D_pFuncCallback.size();
        if (size > m_iCurFrameIdx)
        {
            size = m_pCurAnim->vec2D_pFuncCallback[m_iCurFrameIdx].size();
            for (size_t i = 0; i < size; ++i)
            {
                //함수 호출
                if(nullptr != m_pCurAnim->vec2D_pFuncCallback[m_iCurFrameIdx][i])
                    m_pCurAnim->vec2D_pFuncCallback[m_iCurFrameIdx][i]();
            }
        }
    }

    //재생할 애니메이션이 없더라도 업데이트는 해준다.(재생할 애니메이션이 없다고 업데이트)
    UpdateData();
}

void cCom_Animator2D::cleanup()
{
    Stop();
}

void cCom_Animator2D::UpdateData()
{
    UpdateAtlasTexToMtrl();

    //재생할 애니메이션이나 선택된 애니메이션이 없을 시 애니메이션 사용 설정을 해제하고 return
    cGameObject* pOwner = GetOwner();

    //기존의 플래그값을 받아와서 플래그를 꺼 준뒤 데이터를 다시 설정해준다.
    int iAnimFlag = pOwner->GetMtrlScalarParam_Int(MTRL_SCALAR_STD2D_FLAG);
    if (nullptr == m_pCurAnim)
    {
        iAnimFlag &= ~((int)eMTRL_SCALAR_STD2D_FLAG::USE_ANIM);
        iAnimFlag &= ~((int)eMTRL_SCALAR_STD2D_FLAG::USE_PIVOT);

        pOwner->SetMtrlScalarParam(MTRL_SCALAR_STD2D_FLAG, &iAnimFlag);
        pOwner->SetMtrlScalarParam(MTRL_SCALAR_STD2D_ANIM_TEXATLAS_IDX, &m_iCurAtlasTexIdx);

        union union_PackINT32
        {
            INT16 High;
            INT16 Low;
        };
        //union_PackInt32 pack = 

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

    pOwner->SetMtrlScalarParam(MTRL_SCALAR_STD2D_ANIM_UV_LEFTTOP, &(frameuv.v2_UVLeftTop));
    pOwner->SetMtrlScalarParam(MTRL_SCALAR_STD2D_ANIM_UV_SLICE, &(frameuv.v2_UVSlice));
    pOwner->SetMtrlScalarParam(MTRL_SCALAR_STD2D_ANIM_UV_OFFSET, &(frameuv.v2_Offset));
    pOwner->SetMtrlScalarParam(MTRL_SCALAR_STD2D_PIVOT, &(m_pCurAnim->vPivot));
}


void cCom_Animator2D::AddAtlasTex(eMTRLDATA_PARAM_TEX _eTexParam, Ptr<cAnim2DAtlas> _pAtlasTex)
{
    m_arrAtlasTex[(int)_eTexParam] = _pAtlasTex;
    m_bNeedUpdateMtrl = true;
}


const tAnim2D* cCom_Animator2D::FindAnim(const string_view _strKey_Anim)
{
    const tAnim2D* Anim = nullptr;

    for (int i = 0; i < MAXNUM_ANIM; ++i)
    {
        if (nullptr != m_arrAtlasTex[i])
        {
            Anim = m_arrAtlasTex[i]->FindAnim2D(_strKey_Anim);

            if (Anim)
                break;
        }
    }

    return Anim;
}

bool cCom_Animator2D::Play(const string_view _strKey_Anim, eANIM_PLAYMODE _ePlayMode, bool _bReverse)
{
    const tAnim2D* pAnim = nullptr;

    for (int i = 0; i < MAXNUM_ANIM; ++i)
    {
        if (nullptr == m_arrAtlasTex[i])
            continue;

        pAnim = m_arrAtlasTex[i]->FindAnim2D(_strKey_Anim);
        if (pAnim)
        {
            m_iCurAtlasTexIdx = i;
            break;
        }
    }

    if (nullptr == pAnim)
        return false;

    //동일한 애니메이션에 대해 재생을 요청했을 경우 그냥 냅둠(true 반환)
    else if (pAnim == m_pCurAnim)
        return true;

    //예외처리 완료. 애니메이션 전환
    m_pCurAnim = pAnim;


    //재생 준비
    m_ePlayMode = _ePlayMode;
    m_bReverse = _bReverse;

    m_fCurTime = 0.f;
    m_bFinish = false;
    m_bFlipX = false;

    m_uMaxFrameCount = m_pCurAnim->uNumFrame;

    if (false == m_bReverse)
        m_iCurFrameIdx = 0;
    else
        m_iCurFrameIdx = (int)m_uMaxFrameCount - 1;

    m_fTimePerFrame = m_pCurAnim->fTimePerFrame;
    m_fFullPlayTime = m_pCurAnim->fFullPlayTime;
        
    if (m_arrAtlasTex[m_iCurAtlasTexIdx]->IsFrameSizeRegular())
    {
        Transform().SetSize(Vec3(m_arrAtlasTex[m_iCurAtlasTexIdx]->GetFrameSize(0u), 1.f));
    }

    RenderComponent()->SetDisable(false);
    SetDisable(false);
            
    //재생준비 완료 - true 반환
    return true;
}

bool cCom_Animator2D::PreparePlay(const string_view _strKey_Anim, eANIM_PLAYMODE _ePlayMode, bool _bReverse)
{
    const tAnim2D* pAnim = nullptr;

    for (int i = 0; i < MAXNUM_ANIM; ++i)
    {
        if (nullptr == m_arrAtlasTex[i])
            continue;

        pAnim = m_arrAtlasTex[i]->FindAnim2D(_strKey_Anim);
        if (pAnim)
        {
            m_iCurAtlasTexIdx = i;
            break;
        }
    }

    if (nullptr == pAnim)
        return false;

    //동일한 애니메이션에 대해 재생을 요청했을 경우 그냥 냅둠(true 반환)
    else if (pAnim == m_pCurAnim)
        return true;

    //예외처리 완료. 애니메이션 전환
    m_pCurAnim = pAnim;


    //재생 준비
    m_ePlayMode = _ePlayMode;
    m_bReverse = _bReverse;

    m_fCurTime = 0.f;
    m_bFinish = true;
    m_bFinishChecked = true;
    m_bFlipX = false;

    m_uMaxFrameCount = m_pCurAnim->uNumFrame;

    if (false == m_bReverse)
        m_iCurFrameIdx = 0;
    else
        m_iCurFrameIdx = (int)m_uMaxFrameCount - 1;

    m_fTimePerFrame = m_pCurAnim->fTimePerFrame;
    m_fFullPlayTime = m_pCurAnim->fFullPlayTime;

    if (m_arrAtlasTex[m_iCurAtlasTexIdx]->IsFrameSizeRegular())
    {
        Transform().SetSize(Vec3(m_arrAtlasTex[m_iCurAtlasTexIdx]->GetFrameSize(0u), 1.f));
    }

    RenderComponent()->SetDisable(true);
    SetDisable(true);

    //재생준비 완료 - true 반환
    return true;
}

void cCom_Animator2D::PlayAgain()
{
    if (false == m_bReverse)
        m_iCurFrameIdx = 0;
    else
        m_iCurFrameIdx = (int)m_uMaxFrameCount - 1;

    m_fCurTime = 0.f;
    m_bFinish = false;

    RenderComponent()->SetDisable(false);
    SetDisable(false);
}

const Vec2 cCom_Animator2D::GetCurFrameSize() 
{
    Vec2 size;
    if (m_pCurAnim && m_arrAtlasTex[0].Get())
    {
        const Vec2& TexSize = m_arrAtlasTex[0]->GetAtlasTex()->GetSize();
        const Vec2& SliceSize = m_arrAtlasTex[0]->GetFrameUVData(m_pCurAnim->vecFrame[0]).v2_UVSlice;
        size = TexSize * SliceSize;
    }
    return size;
}


void cCom_Animator2D::UpdateAtlasTexToMtrl()
{
    if (false == m_bNeedUpdateMtrl)
        return;

    cMaterial* pMtrl = GetOwner()->RenderComponent()->GetCurMaterial().Get();

    for (int i = 0; i < MAXNUM_ANIM; ++i)
    {
        Ptr<cTexture> pTex = nullptr;
        if (nullptr != m_arrAtlasTex[i])
            pTex = m_arrAtlasTex[i]->GetAtlasTex();

        pMtrl->SetTexParam((eMTRLDATA_PARAM_TEX)i, pTex);
    }

    m_bNeedUpdateMtrl = false;
}

void cCom_Animator2D::CalculateDirectionalColHalfFlipAtlas()
{
    //방향의 갯수를 구한다. 12시 방향과 6시 방향은 flip할 시 중복되므로 2를 빼준다.
    int NumDirection = m_pCurAnim->uColTotal * 2 - 2;
    
    ////라디안 기준 한바퀴(2PI)에서 방향의 갯수만큼 나눠준다. 한 방향당 각도이다.
    float fAngleSlice = XM_2PI / (float)NumDirection;

    //현재 z축 회전 각도를 받아와서 360도(2PI)로 나눈 나머지를 구해준다.
    //12시 기준이고, 음수는 11시 방향 360도, 양수는 1시 방향 360도를 의미한다.
    float angle = fmodf(-Transform().GetRelativeRot().z, XM_2PI);

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

    m_uCalculatedIdx = m_pCurAnim->vecFrame[idx * m_pCurAnim->uRowTotal + m_iCurFrameIdx];
}
