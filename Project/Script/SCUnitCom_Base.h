#pragma once
#include <Engine/CEntity.h>

enum class eSCUnit_Component
{

};

class CSC_Entity;
//유닛 컴포넌트 기초 클래스
class SCUnitCom_Base
    : public CEntity
{
public:
    SCUnitCom_Base();

    SCUnitCom_Base(const SCUnitCom_Base& _other) = default;
    CLONE_DISABLE(SCUnitCom_Base);

    virtual ~SCUnitCom_Base();

private:
    DECLARE_GETSET(CSC_Entity*, m_pOwner, Owner);
};

