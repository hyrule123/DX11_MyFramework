#pragma once
#include <Engine/CEntity.h>

#include "define_SCUnit.h"

class CScript_SCEntity;
//유닛 컴포넌트 기초 클래스
class SCUnitCom_Base
    : public CEntity
{
public:
    SCUnitCom_Base(CScript_SCEntity* _pOwner);

    SCUnitCom_Base(const SCUnitCom_Base& _other);
    CLONE_DISABLE(SCUnitCom_Base);

    virtual ~SCUnitCom_Base();

public:
    virtual void init() = 0;
    virtual void tick() = 0;
    
    
private:
    DECLARE_GETTER_SETTER(CScript_SCEntity*, m_pOwner, Owner);
};
