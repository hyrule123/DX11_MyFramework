#pragma once

#include <Engine/CScript.h>

class CScript_MouseCursor :
    public CScript
{
public:
    CScript_MouseCursor();
    virtual ~CScript_MouseCursor();
    CLONE(CScript_MouseCursor);

private:
    

public:
    virtual void tick() override;
    virtual void OnCollision(CCollider* _Collider) override;

};

