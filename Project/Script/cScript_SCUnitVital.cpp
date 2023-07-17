#include "pch.h"


#include "cScript_SCUnitVital.h"

cScript_SCUnitVital::cScript_SCUnitVital(const string_view _strKey)
    : IScript(_strKey)
    , m_uMaxHP(100u)
    , m_fCurHP()
    , m_uMaxShield()
    , m_fCurShield()
    , m_strArmorName()
    , m_uDefaultArmor()
    , m_uArmorAddedPerUpgrade(1u)
    , m_eArmorType()
{
    m_fCurHP = (float)m_uMaxHP;
}

cScript_SCUnitVital::~cScript_SCUnitVital()
{
}
