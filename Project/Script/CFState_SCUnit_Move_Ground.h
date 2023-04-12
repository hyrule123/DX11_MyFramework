#pragma once

#include "CFState_SCUnit_Move_Base.h"
#include "define_SCUnit.h"

class CFState_SCUnit_Move_Ground :
    public CFState_SCUnit_Move_Base
{
public:
    CFState_SCUnit_Move_Ground();
    virtual ~CFState_SCUnit_Move_Ground();

    CFState_SCUnit_Move_Ground(const CFState_SCUnit_Move_Ground& _other) = default;
    CLONE(CFState_SCUnit_Move_Ground);

public:
    virtual void init() override;

    virtual void EnterState() override;
    virtual void OnState() override;
    virtual void EndState() override;

    //상태 변경을 요청한 State의 번호
    //상태 변경이 가능할 경우 true를 반환해 주면 상태를 변경시킬 수 있다.
    virtual bool CheckCondition(UINT _eState) override { return true; }


};

