#pragma once
#include <Engine/I_FSM.h>

class cScript_FSM_Death :
    public I_FSM
{
public:
    cScript_FSM_Death(const string_view _strKey);

    cScript_FSM_Death(const cScript_FSM_Death& _other) = default;
    CLONE(cScript_FSM_Death);

    virtual ~cScript_FSM_Death();

public:
    virtual void EnterState(const tFSM_Event& _tEvent) override;
    virtual void OnState() override;
    virtual void EndState() override;

    //상태 변경을 요청한 State의 번호
    //상태 변경이 가능할 경우 true를 반환해 주면 상태를 변경시킬 수 있다.
    //Death의 경우 한번 들어오면 다시 되돌리기 불가능
    virtual eFSM_RESULT CheckCondition(const tFSM_Event& _tEvent) override { return eFSM_RESULT::REJECT; }
};

