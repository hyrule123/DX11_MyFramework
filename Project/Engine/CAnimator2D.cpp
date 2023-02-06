#include "pch.h"
#include "CAnimator2D.h"

#include "CAnim2D.h"

#include "CMeshRender.h"
#include "CMaterial.h"

CAnimator2D::CAnimator2D()
    : CComponent(eCOMPONENT_TYPE::ANIMATOR2D)
    , m_pCurAnim()
    , m_bRepeat()
{
}

CAnimator2D::~CAnimator2D()
{
    for (const auto& iter : m_mapAnim)
    {
        if (nullptr != iter.second)
            delete iter.second;
    }
}

void CAnimator2D::finaltick()
{
    if (nullptr != m_pCurAnim)
    {
        if (m_bRepeat && m_pCurAnim->IsFinish())
        {
            m_pCurAnim->Reset();
        }

        m_pCurAnim->finaltick();
    }
}

void CAnimator2D::UpdateData()
{
    CMaterial* pMtrl = MeshRender()->GetMaterial().Get();

    const tAnim2DFrm& frm = m_pCurAnim->GetCurFrame();
    Vec2 vBackSize = m_pCurAnim->GetBackSize();

    int iAnimUse = 1;
    pMtrl->SetScalarParam(INT_0, &iAnimUse);
    pMtrl->SetScalarParam(VEC2_0, &frm.LeftTopUV);
    pMtrl->SetScalarParam(VEC2_1, &frm.SliceUV);
    pMtrl->SetScalarParam(VEC2_2, &frm.Offset);
    pMtrl->SetScalarParam(VEC2_3, &vBackSize);

    pMtrl->SetTexParam(eTEX_0, m_pCurAnim->GetAtlasTex());
}

void CAnimator2D::Play(const string& _strName, bool _bRepeat)
{
    CAnim2D* pAnim = FindAnim(_strName);
    assert(nullptr != pAnim);

    m_pCurAnim = pAnim;
    m_bRepeat = _bRepeat;
}

CAnim2D* CAnimator2D::FindAnim(const string& _strName)
{
    const auto& iter = m_mapAnim.find(_strName);

    if (iter == m_mapAnim.end())
        return nullptr;

    return iter->second;
}

void CAnimator2D::CreateAnimation(const string& _strAnimName, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _vBackSize, int _FrameCount, int _FPS)
{
    CAnim2D* pAnim = new CAnim2D;
    pAnim->Create(_strAnimName, _AtlasTex, _vLeftTop, _vSlice, _vBackSize, _FrameCount, _FPS);

    pAnim->m_pOwner = this;
    m_mapAnim.insert(make_pair(_strAnimName, pAnim));
}
