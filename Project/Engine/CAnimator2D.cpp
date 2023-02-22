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
    //����� �ִϸ��̼��� ���ų� �ִϸ��̼� ��� ����� return
    if (nullptr == m_pCurAnim || true == m_bFinish)
        return;

    //�ð� �߰�
    m_fCurTime += DELTA_TIME;


    //�����Ӵ� ����ð��� �Ѿ�� ���
    if (m_fTimePerFrame < m_fCurTime)
    {
        //ftime���� �������Ӻ� �ð��� ������
        m_fCurTime -= m_fTimePerFrame;

        //���� ���������� �����Ѵ�.
        ++m_uCurFrame;

        //������ �������� ���
        if (m_uMaxFrameCount <= m_uCurFrame)
        {
            //������ ���������� ������Ų��
            m_uCurFrame = m_uMaxFrameCount - 1;
            m_bFinish = true;
        }
    }

    //����� ��� �����µ�
    if (true == m_bFinish)
    {
        //������̳� ������ �����Ǿ����� ���
        switch (m_eLoopMode)
        {
        case eANIM_LOOPMODE::NONE:
            //���ٰ� ����
            break;
        case eANIM_LOOPMODE::NORMAL_LOOP:
            //�������� �ٽ� 0������ ����
            m_uCurFrame = 0u;

            m_bFinish = false;
            break;
        case eANIM_LOOPMODE::ZIG_ZAG:
            m_uCurFrame = 0u;

            //������ 0������ �����ϰ� ������ ��带 �ݴ�� �ٲ�
            m_bReverse = !m_bReverse;

            m_bFinish = false;
            break;
        default:
            break;
        }
    }


    //��������� ���θ� ���� ���
    if (true == m_bReverse)
        m_uCurFrameIdx = m_uMaxFrameCount - 1u - m_uCurFrame;
    else
        m_uCurFrameIdx = m_uCurFrame;

    //���߿� ���⼭ ���⼺�� ������ ��Ұ����� ����� �߰��Ұ�
    m_uCalculatedIdx = m_pCurAnim->vecFrame[m_uCurFrameIdx].uIdx;
    
    //�����ӿ� ��ϵ� �ݹ��Լ��� ���� ��� �ݹ��Լ� ȣ��
    size_t size = m_pCurAnim->vecFrame[m_uCurFrameIdx].pfuncCallback.size();
    if ((size_t)0 != size)
    {
        for (size_t i = 0; i < size; i++)
        {
            //�ݹ��Լ��� ȣ��
            m_pCurAnim->vecFrame[m_uCurFrameIdx].pfuncCallback[i]();
        }
    }
}

void CAnimator2D::finaltick()
{
    //����� �ִϸ��̼��� ���� �� 
    if (nullptr == m_pCurAnim)
    {
        m_uCalculatedIdx = -1;
        return;
    }
        

    UpdateData();
}

void CAnimator2D::UpdateData()
{
    //�ִϸ��̼��� UV ������ �޾ƿ�.
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