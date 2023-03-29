#pragma once
#include "CFSM.h"

#include "SCGroundUnit.h"

class CScript_SCGroundUnitIdle :
    public CFSM
{
public:
    CScript_SCGroundUnitIdle();
    virtual ~CScript_SCGroundUnitIdle();
    CLONE(CScript_SCGroundUnitIdle);

public:
    virtual void init() override;

    virtual void EnterState() override;
    virtual void OnState() override;
    virtual void EndState() override;

    //상태 변경을 요청한 State의 번호
    //상태 변경이 가능할 경우 true를 반환해 주면 상태를 변경시킬 수 있다.
    virtual bool CheckCondition(UINT _eState) override { return true; }
};

