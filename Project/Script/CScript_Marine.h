#pragma once
#include "CScript_SCEntity.h"

#include "define_SCUnit.h"
class CScript_Marine :
    public CScript_SCEntity
{
public:
    CScript_Marine(const string& _strKey);
    virtual ~CScript_Marine();
    CLONE(CScript_Marine);

public:
    void Idle()     { ScriptHolder()->Transition(FSM_SCGroundUnit::IDLE); }
    void Move()     { ScriptHolder()->Transition(FSM_SCGroundUnit::MOVE); }
    void Attack()   { ScriptHolder()->Transition(FSM_SCGroundUnit::ATTACK_BEGIN); }
};

