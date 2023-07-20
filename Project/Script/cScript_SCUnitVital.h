#pragma once
#include "cScript_SCEntity.h"
class cScript_SCUnitVital :
    public IScript
{
public:
    cScript_SCUnitVital();
    virtual ~cScript_SCUnitVital();

    cScript_SCUnitVital(const cScript_SCUnitVital& _other) = default;
    CLONE(cScript_SCUnitVital);

private:
    DECLARE_GETTER_SETTER(UINT, m_uMaxHP, MaxHP);
    DECLARE_GETTER_SETTER(float, m_fCurHP, CurHP);

    DECLARE_GETTER_SETTER(UINT, m_uMaxShield, MaxShield);
    DECLARE_GETTER_SETTER(float, m_fCurShield, CurShield);

private:
    //아머
    //업그레이드 키를 찾는데도 사용함.
    string m_strArmorName;
public:
    const string& GetArmorName() const { return m_strArmorName; }

private:
    DECLARE_GETTER_SETTER(UINT, m_uDefaultArmor, DefaultArmor);
    DECLARE_GETTER_SETTER(UINT, m_uArmorAddedPerUpgrade, ArmorAddedPerUpgrade);

    DECLARE_GETTER_SETTER(SC::eUNIT_ARMOR_TYPE, m_eArmorType, ArmorType);
};

