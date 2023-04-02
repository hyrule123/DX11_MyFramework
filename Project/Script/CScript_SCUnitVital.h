#pragma once
#include "CSCEntity.h"
class CScript_SCUnitVital :
    public CScript
{
public:
    CScript_SCUnitVital();
    virtual ~CScript_SCUnitVital();

    CScript_SCUnitVital(const CScript_SCUnitVital& _other);
    CLONE(CScript_SCUnitVital);

private:
    //체력 관련
    UINT m_uHealthMax;
    UINT m_uCurHealth;
    UINT m_uShildMax;
    UINT m_uCurShild;

    //아머
    //업그레이드 키를 찾는데도 사용함.
    string m_strArmorName;
    UINT m_uDefaultArmor;
    UINT m_uArmorAddedPerUpgraded;

    eSCUNIT_ARMOR_TYPE m_eArmorType;

};

