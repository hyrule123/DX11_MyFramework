#pragma once
#include "CEntity.h"

#include "ptr.h"

class CTexture;
class CAnimator2D;

class CAnim2D :
    public CEntity
{
public:
    CAnim2D();
    ~CAnim2D();
    CLONE(CAnim2D);
    friend class CAnimator2D;

public:
    void finaltick();

private:
    CAnimator2D* m_pOwner;


    

    int                 m_iCurFrm;
    float               m_fTime;

    bool                m_bFinish;

public:


    bool IsFinish() { return m_bFinish; }
    void Reset()
    {
        m_iCurFrm = 0;
        m_fTime = 0.f;
        m_bFinish = false;
    }
};

