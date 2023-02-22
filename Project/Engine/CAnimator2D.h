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
    CAnimator2D(const CAnimator2D& _other);
    virtual ~CAnimator2D();
    CLONE(CAnimator2D);

public:
    virtual void tick() override;
    virtual void finaltick() override;

    virtual void cleanup() override {};

    void UpdateData();
    

private:
    //0���� üũ�Ǵ� ������ ��ȣ
    UINT                m_uCurFrame;

    //���Ϳ��� ã�� ���� �������� �ε��� ��ȣ
    UINT                m_uCurFrameIdx;

    //������ ���� vecFrameUV �ȿ����� �ε���
    int                m_uCalculatedIdx;

    float               m_fCurTime;
    bool                m_bFinish;

    UINT                m_uMaxFrameCount;
    float               m_fTimePerFrame;
    float               m_fFullPlayTime;  

    eANIM_LOOPMODE      m_eLoopMode;
    bool                m_bReverse;

    Ptr<CAnim2DAtlas>       m_arrAtlasTex[(int)eMTRLDATA_PARAM_TEX::_END];

    //���� ��� ����
    int                     m_iCurAtlasTexIdx;
    const tAnimFrameIdx*    m_pCurAnim;

    bool                    m_bNeedUpdateMtrl;

    //TODO : ���� �ݹ��Լ��� ����ϰ��� �Ҷ��� Script�� �����ǵ��� ���ٰ�

public:
    //�ִϸ��̼� �����ʹ� �ؽ�ó�� ������ �̸��� ����Ұ�
    void AddAtlasTex(eMTRLDATA_PARAM_TEX _eTexParam, Ptr<CAnim2DAtlas> _pAtlasTex);
    void Play(const string& _strAnimName, eANIM_LOOPMODE _eLoopMode, bool _bReverse);

    
private:
    void Stop() { m_iCurAtlasTexIdx = -1; m_pCurAnim = nullptr; };
    void UpdateAtlasTexToMtrl();
};

