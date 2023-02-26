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
    //����� �ִϸ��̼��� ���� ��� ������ ó��
    if (nullptr != m_pCurAnim)
    {
        //����� ������ �ʾ��� ��� ���� ������ ������Ʈ
        if (false == m_bFinish)
        {
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

            //����� �̹� �����ӿ� ����Ǿ��� ��� ���� ������ ó��
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
        }


        //���� �ִϸ��̼� �������� ����Ѵ�.
        //��������� ���θ� ���� ���
        if (true == m_bReverse)
            m_uCurFrameIdx = m_uMaxFrameCount - 1u - m_uCurFrame;
        else
            m_uCurFrameIdx = m_uCurFrame;


        //������ ���� �ִϸ��̼��� ��� ���⿡ ���� �������� ����Ѵ�.
        if (eANIM_TYPE::DIRECTIONAL_COL_HALF_FLIP == m_pCurAnim->eAnimType)
        {
            CalculateDirectionalColHalfFlipAtlas();
        }
        else if (eANIM_TYPE::SEQUENTIAL == m_pCurAnim->eAnimType)
        {
            m_uCalculatedIdx = m_pCurAnim->vecFrame[m_uCurFrameIdx].uIdxInVecFrameUV;
        }


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

    //����� �ִϸ��̼��� ������ ������Ʈ�� ���ش�.(����� �ִϸ��̼��� ���ٰ� ������Ʈ)
    UpdateData();
}

void CAnimator2D::UpdateData()
{
    UpdateAtlasTexToMtrl();

    //����� �ִϸ��̼��̳� ���õ� �ִϸ��̼��� ���� �� �ִϸ��̼� ��� ������ �����ϰ� return
    CGameObject* pOwner = GetOwner();

    //������ �÷��װ��� �޾ƿͼ� �÷��׸� �� �ص� �����͸� �ٽ� �������ش�.
    int iAnimFlag = pOwner->GetMtrlScalarParam_Int(MTRL_SCALAR_STD2D_FLAG);
    if (nullptr == m_pCurAnim)
    {
        iAnimFlag &= ~((int)eMTRL_SCALAR_STD2D_FLAG::USEANIM);
        iAnimFlag &= ~((int)eMTRL_SCALAR_STD2D_FLAG::USEPIVOT);

        pOwner->SetMtrlScalarParam(MTRL_SCALAR_STD2D_FLAG, &iAnimFlag);
        pOwner->SetMtrlScalarParam(MTRL_SCALAR_STD2D_ANIM_TEXATLAS_IDX, &m_iCurAtlasTexIdx);
        return;
    }

    //�÷��װ� �߰� �� ����
    iAnimFlag |= (int)eMTRL_SCALAR_STD2D_FLAG::USEANIM;

    if (Vec2(0.5f, 0.5f) != m_pCurAnim->vPivot)
        iAnimFlag |= (int)eMTRL_SCALAR_STD2D_FLAG::USEPIVOT;
    else
        iAnimFlag &= ~(int)eMTRL_SCALAR_STD2D_FLAG::USEPIVOT;

    if (true == m_bFlipX)
        iAnimFlag |= (int)eMTRL_SCALAR_STD2D_FLAG::NEEDFLIPX;
    else
        iAnimFlag &= ~(int)eMTRL_SCALAR_STD2D_FLAG::NEEDFLIPX;
    

    pOwner->SetMtrlScalarParam(MTRL_SCALAR_STD2D_FLAG, &iAnimFlag);
    pOwner->SetMtrlScalarParam(MTRL_SCALAR_STD2D_ANIM_TEXATLAS_IDX, &m_iCurAtlasTexIdx);
 

    //�ִϸ��̼��� UV ������ �޾ƿ�.
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
        m_bFlipX = false;

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

void CAnimator2D::CalculateDirectionalColHalfFlipAtlas()
{
    //������ ������ ���Ѵ�. 12�� ����� 6�� ������ flip�� �� �ߺ��ǹǷ� 2�� ���ش�.
    int NumDirection = m_pCurAnim->uColTotal * 2 - 2;
    
    ////���� ���� �ѹ���(2PI)���� ������ ������ŭ �����ش�. �� ����� �����̴�.
    float fAngleSlice = XM_2PI / (float)NumDirection;

    //���� z�� ȸ�� ������ �޾ƿͼ� 360��(2PI)�� ���� �������� �����ش�.
    //12�� �����̰�, ������ 11�� ���� 360��, ����� 1�� ���� 360���� �ǹ��Ѵ�.
    float angle = fmodf(-Transform()->GetRelativeRot().z, XM_2PI);

    //������ ������ ���(�ݽð���� ȸ���� ���) 2PI���� ����(angle�� �����̹Ƿ� �����ָ� ��) ��� �������� �ٲ��ش�.
    //�̷��� 0 ~ 360�� ������ 0 ~ 2PI ������ �ٲ�� �ȴ�.
    if (angle < 0.f)
        angle = XM_2PI + angle;
    
    //������ angle���� ������ ������ŭ �����ָ�, �ε��� ��ȣ�� float���� ������ �ȴ�.
    angle /= fAngleSlice;

    //fabsf()�� ���� ���� �ݿø����ش�. �׷��� 12�� ������ �����ȴ�.
    angle = roundf(fabsf(angle));

    //angle�� ���� ������ ����(uColTotal)�� ���� �ָ� ��Ʋ�󽺿� ���� ������ ���� �� �ִ�

    //�� ���� ������ �ٲ۴�.
    UINT idx = (UINT)angle;

    //���� �ε����� ���� �Ѿ ��� ���� ���ְ� flip ���¸� true�� �����Ѵ�.

    if (idx >= m_pCurAnim->uColTotal)
    {
        idx = NumDirection - idx;

        //12�� ����� 6�� ���� ��������Ʈ�� �ø����� ����.
        //if (idx == m_pCurAnim->uColTotal - 1u || idx == 0)
        //    m_bFlipX = false;
        //else
            m_bFlipX = true;
    }
    else
    {
        m_bFlipX = false;
    }

    m_uCalculatedIdx = m_pCurAnim->vecFrame[idx * m_pCurAnim->uRowTotal + m_uCurFrameIdx].uIdxInVecFrameUV;
}
