#pragma once
#include <Engine/CEntity.h>

#include "define_SCUnit.h"

class CSC_Entity;
//유닛 컴포넌트 기초 클래스
class SCUnitCom_Base
    : public CEntity
{
public:
    SCUnitCom_Base(eSCUnit_ComType _eComType, CSC_Entity* _pOwner);

    SCUnitCom_Base(const SCUnitCom_Base& _other);
    CLONE_DISABLE(SCUnitCom_Base);

    virtual ~SCUnitCom_Base();

public:
    virtual void init() = 0;
    virtual void tick() = 0;
    
    
private:
    DECLARE_GETSET(eSCUnit_ComType, m_eSCUnit_ComType, ComType);
    DECLARE_GETSET(CSC_Entity*, m_pOwner, Owner);
};

