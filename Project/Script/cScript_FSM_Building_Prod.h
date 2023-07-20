#pragma once

#include "cScript_FSM_Idle.h"

//Idle 기반의 생산건물용 FSM
class cScript_FSM_Building_Prod :
    public cScript_FSM_Idle
{
public:
    cScript_FSM_Building_Prod(const string_view _strKey);
    virtual ~cScript_FSM_Building_Prod();

    cScript_FSM_Building_Prod(const cScript_FSM_Building_Prod& _other) = default;
    CLONE(cScript_FSM_Building_Prod);

    virtual void Tick() override;

private:
    //유닛 생산중인지 여부 저장
    DECLARE_GETTER_SETTER(bool, m_bIsProducing, IsProducing);

    bool m_bIsFlickerOn;
    float m_fFlickerTime;

private:

};

