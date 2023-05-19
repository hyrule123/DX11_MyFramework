#pragma once
#include <Engine/CFSM.h>

class CFState_SCUnit_Death :
    public CFSM
{
public:
    CFState_SCUnit_Death();

    CFState_SCUnit_Death(const CFState_SCUnit_Death& _other) = default;
    CLONE(CFState_SCUnit_Death);

    virtual ~CFState_SCUnit_Death();

public:
    virtual void EnterState() override;
    virtual void OnState() override;
    virtual void EndState() override;

    //상태 변경을 요청한 State의 번호
    //상태 변경이 가능할 경우 true를 반환해 주면 상태를 변경시킬 수 있다.
    virtual bool CheckCondition(UINT _eState) override;
};

