#pragma once
#include "CSCEntity.h"

#include "define_SCUnit.h"
class CScript_Marine :
    public CSCEntity
{
public:
    CScript_Marine();
    virtual ~CScript_Marine();
    CLONE(CScript_Marine);

public:
    void Idle()     { Transition((UINT)FSM_SCGroundUnit::eSTATE::IDLE); }
    void Move()     { Transition((UINT)FSM_SCGroundUnit::eSTATE::MOVE); }
    void Attack()   { Transition((UINT)FSM_SCGroundUnit::eSTATE::ATTACK); }
};

