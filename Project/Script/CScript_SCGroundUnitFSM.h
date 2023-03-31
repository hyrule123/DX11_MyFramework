#pragma once

#include <Engine/CFStateMgr.h>
#include <Engine/CKeyMgr.h>

#include "defineFSM_SCGroundUnit.h"



enum class eSCUNIT_UNITSIZE_TYPE
{
    Small,
    Midium,
    Big,
};

//두 상태를 동시에 가질 수 있으므로 
enum class eSCUNIT_ARMOR_TYPE
{
    isBionic = BITMASK(0),
    isMechanic = BITMASK(1),
};


enum class eSCUNIT_ATTACK_TYPE
{
    Normal,
    Explosive,
    Concussive,
    Spell
};

enum class eSCUNIT_WEAPON_FLAG
{
    isGroundAttackable = BITMASK(0),
    isAirAttackable = BITMASK(1),
};

class CScript_SCGroundUnitFSM :
    public CFStateMgr
{
public:
    CScript_SCGroundUnitFSM();
    //CScript_SCGroundUnitFSM(const CScript_SCGroundUnitFSM& _other); //기본 복사생성자로 충분함.
    ~CScript_SCGroundUnitFSM();

    CLONE(CScript_SCGroundUnitFSM);

public:
    virtual void initFSM() override;


public:
    virtual void OnCollision(CCollider* _pCol) override;

private:
    //시야. 32를 곱해줘야 함
    UINT m_uSightRange;

    UINT m_uCostMineral;
    UINT m_uCostGas;

    UINT m_uPopulation;
    float m_BaseBuildTime;

    eSCUNIT_UNITSIZE_TYPE m_eUnitSize;


    //체력 관련
    UINT m_uHealthMax;


    string m_strProdBuildingName;
    //UI의 몇 번 칸에 출력되는지
    UINT m_ProdSlot;
    eKEY m_eKeyShortcut;
    //생산 조건 flag(테크) - 나중에 사용 예정
    UINT m_flagTech;


    //무기
    string m_strWeaponName;
    UINT m_uDefaultDamage;
    UINT m_uDamageAddedPerUpgrade;
    UINT m_uWeaponRange;

    //아머
    string m_strArmorName;
    UINT m_uDefaultArmor;
    UINT m_uArmorAddedPerUpgraded;

    eSCUNIT_ARMOR_TYPE m_eArmorType;
  

public:


private:

};

