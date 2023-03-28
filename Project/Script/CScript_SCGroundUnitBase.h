#pragma once

#include "CFSMMgr.h"

#include "SCGroundUnit.h"


class CScript_SCGroundUnitBase :
    public CFSMMgr
{
public:
    CScript_SCGroundUnitBase();
    //CScript_SCGroundUnitBase(const CScript_SCGroundUnitBase& _other); //기본 복사생성자로 충분함.
    ~CScript_SCGroundUnitBase();

    CLONE(CScript_SCGroundUnitBase);

public:
    virtual void init() override;


public:
    virtual void OnCollision(CCollider* _pCol) override;

private:
    Vec4 m_ColorKey;
    float m_MoveSpeed;
    float m_TurningForceRad;
    SCGroundUnit::eSTATE m_eCurState;
    SCGroundUnit::eSTATE m_ePrevState;

    //TODO : 마우스 클릭 확인을 위한 임시 변수
    bool m_bSelected;

public:
    bool IsSelected() const { return m_bSelected; }


private:

};

