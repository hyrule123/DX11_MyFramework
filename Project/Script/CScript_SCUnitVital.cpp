#include "pch.h"


#include "CScript_SCUnitVital.h"

CScript_SCUnitVital::CScript_SCUnitVital(const string& _strKey)
    : CScript(_strKey)
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

CScript_SCUnitVital::~CScript_SCUnitVital()
{
}
