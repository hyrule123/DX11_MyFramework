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
    ~CAnimator2D();
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

    Ptr<CAnim2DAtlas>    m_pCurAnimSprite;
    const tAnimFrameIdx*  m_pCurAnim;
    //TODO : ���� �ݹ��Լ��� ����ϰ��� �Ҷ��� Script�� �����ǵ��� ���ٰ�

public:
    //�ִϸ��̼� �����ʹ� �ؽ�ó�� ������ �̸��� ����Ұ�
    void SetAtlasTex(const string& _AtlasTexStrKey);
    void Play(const string& _strName, eANIM_LOOPMODE _eLoopMode, bool _bReverse);

    
private:
    void ResetCurAnim();
};

