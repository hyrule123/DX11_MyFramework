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
    void Idle()     { Transition(FSM_MARINE::IDLE); }
    void Move()     { Transition(FSM_MARINE::MOVE); }
    void Attack()   { Transition(FSM_MARINE::ATTACK_BEGIN); }
};

