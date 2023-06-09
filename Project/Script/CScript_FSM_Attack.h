#pragma once

#include <Engine/CFSM.h>

#include "define_SCUnit.h"

//dest_param: CGameObject* _pTarget
enum class eATTACK_STATE
{
    BEGIN_ATTACK,
    ATTACKING,
    END_ATTACK
};

class CScript_FSM_Attack_BeginEnd;

class CScript_FSM_Attack :
    public CFSM
{
public:
    CScript_FSM_Attack(const string& _strKey);
    virtual ~CScript_FSM_Attack();

    CScript_FSM_Attack(const CScript_FSM_Attack& _other) = default;
    CLONE(CScript_FSM_Attack);

public:
    virtual void EnterState(const tFSM_Event& _tEvent) override;
    virtual void OnState() override;
    virtual void EndState() override;

    //상태 변경을 요청한 State의 번호
    //상태 변경이 가능할 경우 true를 반환해 주면 상태를 변경시킬 수 있다.
    virtual eFSM_RESULT CheckCondition(const tFSM_Event& _tEvent) override;

private:
    void BeginAttack();
    void Attacking();
    void EndAttack();

private:
    //Intantiate, 업그레이드 확인용으로 사용
    string m_strWeaponName;
    GETTER_SETTER(const string&, m_strWeaponName, WeaponName);

private: 
    UINT m_uDefaultDMG;
    GETTER_SETTER(UINT, m_uDefaultDMG, DefaultDMG);

private:
    UINT m_uDMGAddedPerUpgrade;
    GETTER_SETTER(UINT, m_uDMGAddedPerUpgrade, DMGPerUpgrade);

private:
    //공격 사이의 딜레이
    float m_fAtkInterval;
    float m_fCurInterval;
    GETTER_SETTER(float, m_fAtkInterval, AtkInterval);

private:
    //1당 32 픽셀
    UINT m_uWeaponRange;
    GETTER_SETTER(UINT, m_uWeaponRange, WeaponRange);

private:
    //공격 대상
    CGameObject* m_pTarget;
    SETTER(CGameObject*, m_pTarget, Target);

private:
    //현재 공격 상황
    eATTACK_STATE m_eAtkState;

public:
    void SetAll(const string& _strWeaponName, UINT _uDefaultDMG, UINT _uDMGAddedPerUpgrade, UINT _uWeaponRange);
};

inline void CScript_FSM_Attack::SetAll(const string& _strWeaponName, UINT _uDefaultDMG, UINT _uDMGAddedPerUpgrade, UINT _uWeaponRange)
{
    m_strWeaponName = _strWeaponName;
    m_uDefaultDMG = _uDefaultDMG;
    m_uDMGAddedPerUpgrade = _uDMGAddedPerUpgrade;
    m_uWeaponRange = _uWeaponRange;
}


