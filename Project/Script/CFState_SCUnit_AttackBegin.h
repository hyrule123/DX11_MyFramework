#pragma once

#include <Engine/CFSM.h>

class CFState_SCUnit_AttackBegin :
    public CFSM
{
public:
    CFState_SCUnit_AttackBegin();

    CFState_SCUnit_AttackBegin(const CFState_SCUnit_AttackBegin& _other) = default;
    CLONE(CFState_SCUnit_AttackBegin);

    virtual ~CFState_SCUnit_AttackBegin();
    

public:
    virtual void EnterState() override;
    virtual void OnState() override;
    virtual void EndState() override;

    //상태 변경을 요청한 State의 번호
    //상태 변경이 가능할 경우 true를 반환해 주면 상태를 변경시킬 수 있다.
    virtual bool CheckCondition(UINT _eState) override;

};

