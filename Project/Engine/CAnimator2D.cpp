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
    UpdateData();
}

void CAnimator2D::UpdateData()
{
    UpdateAtlasTexToMtrl();

    //����� �ִϸ��̼��̳� ���õ� �ִϸ��̼��� ���� �� �ִϸ��̼� ��� ������ �����ϰ� return
    CGameObject* pOwner = GetOwner();

    //������ �÷��װ��� �޾ƿ�
    int iAnimFlag = pOwner->GetMtrlScalarData_Int(MTRL_SCALAR_STD2D_ANIM_TEXATLAS_IDX);
    if (nullptr == m_pCurAnim)
    {
        iAnimFlag &= ~((int)eANIM2D_FLAG::USEANIM);
        iAnimFlag &= ~((int)eANIM2D_FLAG::USEPIVOT);

        pOwner->SetScalarParam(MTRL_SCALAR_STD2D_FLAG, &iAnimFlag);
        pOwner->SetScalarParam(MTRL_SCALAR_STD2D_ANIM_TEXATLAS_IDX, &m_iCurAtlasTexIdx);
        return;
    }


    //�÷��װ� �߰� �� ����
    iAnimFlag |= (int)eANIM2D_FLAG::USEANIM;
    if (Vec2(0.f, 0.f) != m_pCurAnim->vPivot)
    {
        iAnimFlag |= (int)eANIM2D_FLAG::USEPIVOT;
    }


    pOwner->SetScalarParam(MTRL_SCALAR_STD2D_FLAG, &iAnimFlag);
    pOwner->SetScalarParam(MTRL_SCALAR_STD2D_ANIM_TEXATLAS_IDX, &m_iCurAtlasTexIdx);
 

    //�ִϸ��̼��� UV ������ �޾ƿ�.
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

        //���� �ִϸ��̼��� ��� �ٲ��� ����
        if (curanim == m_pCurAnim)
            return;

        m_pCurAnim = curanim;
    }

    //������ ��ã���� ��� ��ü ��ȸ���鼭 ã�ƺ�
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

    //��� �غ�
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
