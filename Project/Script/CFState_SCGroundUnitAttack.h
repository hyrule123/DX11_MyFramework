#pragma once

#include <Engine/CFState.h>

#include "defineFSM_SCGroundUnit.h"

class CFState_SCGroundUnitAttack :
    public CFState
{
public:
    CFState_SCGroundUnitAttack();
    virtual ~CFState_SCGroundUnitAttack();

public:
    virtual void EnterState() override;
    virtual void OnState() override;
    virtual void EndState() override;

    //상태 변경을 요청한 State의 번호
    //상태 변경이 가능할 경우 true를 반환해 주면 상태를 변경시킬 수 있다.
    virtual bool CheckCondition(UINT _eState) override;

};
