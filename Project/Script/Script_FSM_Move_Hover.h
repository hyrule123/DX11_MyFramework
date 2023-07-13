#pragma once

#include <Engine/CFSM.h>

class CScript_FSM_Move_Hover
	: public CFSM
{
public:
    CScript_FSM_Move_Hover(const string_view _strKey);
    virtual ~CScript_FSM_Move_Hover();

    CScript_FSM_Move_Hover(const CScript_FSM_Move_Hover& _other);
    CLONE(CScript_FSM_Move_Hover);


    //상태 변경을 요청한 State의 번호
//상태 변경이 가능할 경우 true를 반환해 주면 상태를 변경시킬 수 있다.
    virtual eFSM_RESULT CheckCondition(const tFSM_Event& _tEvent) override;

    //호출 시점: Transition()에서 true가 반환될 경우(상태 진입)
    virtual void EnterState(const tFSM_Event& _tEvent) override;

    //호출 시점: 자신이 현재 State일 경우, 스크립트의 tick() 순회 후 호출(상태 진행)
    virtual void OnState() override;

    //호출 시점: Transition()에서 true가 반환될 경우(상태 종료)
    virtual void EndState() override;
};

