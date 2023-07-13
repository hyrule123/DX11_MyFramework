#pragma once

#include "CFSM_Move_Base.h"
#include "define_SC.h"

class CScript_FSM_Move_Ground :
    public CFSM_Move_Base
{
public:
    CScript_FSM_Move_Ground(const string_view _strKey);
    virtual ~CScript_FSM_Move_Ground();

    CScript_FSM_Move_Ground(const CScript_FSM_Move_Ground& _other) = default;
    CLONE(CScript_FSM_Move_Ground);

public:
    virtual void init() override;

    virtual void EnterState(const tFSM_Event& _tEvent) override;
    virtual void OnState() override;
    virtual void EndState() override;

    //상태 변경을 요청한 State의 번호
    //상태 변경이 가능할 경우 true를 반환해 주면 상태를 변경시킬 수 있다.
    //wParam = Destination
    virtual eFSM_RESULT CheckCondition(const tFSM_Event& _tEvent) override { return eFSM_RESULT::ACCEPT; }


};

