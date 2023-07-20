#pragma once

#include "cFSM_Move_Base.h"
#include "define_SC.h"

class cScript_FSM_Move_Ground :
    public cFSM_Move_Base
{
public:
    cScript_FSM_Move_Ground(const string_view _strKey);
    virtual ~cScript_FSM_Move_Ground();

    cScript_FSM_Move_Ground(const cScript_FSM_Move_Ground& _other) = default;
    CLONE(cScript_FSM_Move_Ground);

public:
    virtual void Init() override;

    virtual void EnterState(const tFSM_Event& _tEvent) override;
    virtual void OnState() override;
    virtual void EndState() override;

    //상태 변경을 요청한 State의 번호
    //상태 변경이 가능할 경우 true를 반환해 주면 상태를 변경시킬 수 있다.
    //wParam = Destination
    virtual eFSM_RESULT CheckCondition(const tFSM_Event& _tEvent) override { return eFSM_RESULT::ACCEPT; }


};

