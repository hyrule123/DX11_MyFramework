#pragma once
#include "CSC_Entity.h"

#include "define_SCUnit.h"
class CScript_Marine :
    public CSC_Entity
{
public:
    CScript_Marine();
    virtual ~CScript_Marine();
    CLONE(CScript_Marine);

public:
    void Idle()     { Transition(FSM_SCGroundUnit::IDLE); }
    void Move()     { Transition(FSM_SCGroundUnit::MOVE); }
    void Attack()   { Transition(FSM_SCGroundUnit::ATTACK_BEGIN); }
};

