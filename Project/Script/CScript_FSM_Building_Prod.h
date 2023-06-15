#pragma once
#include "CScript_FSM_Idle.h"

//Idle 기반의 생산건물용 FSM
class CScript_FSM_Building_Prod :
    public CScript_FSM_Idle
{
public:
    CScript_FSM_Building_Prod(const string& _strKey);
    virtual ~CScript_FSM_Building_Prod();

    CScript_FSM_Building_Prod(const CScript_FSM_Building_Prod& _other) = default;
    CLONE(CScript_FSM_Building_Prod);

    virtual void tick() override;

private:
    //유닛 생산중인지 여부 저장
    DECLARE_GETTER_SETTER(bool, m_bIsProducing, IsProducing);

    bool m_bIsFlickerOn;
    float m_fFlickerTime;

private:

};

