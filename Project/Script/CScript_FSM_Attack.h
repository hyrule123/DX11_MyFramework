#pragma once

#include <Engine/CFSM.h>

#include "define_SCUnit.h"

enum class eSCUNIT_ATTACK_STATE
{
    IDLE,
    BEGIN_ATTACK,
    IN_ATTACK,
    END_ATTACK
};

class CScript_FSM_Attack :
    public CFSM
{
public:
    CScript_FSM_Attack(const string& _strKey);
    virtual ~CScript_FSM_Attack();

    CScript_FSM_Attack(const CScript_FSM_Attack& _other) = default;
    CLONE(CScript_FSM_Attack);

public:
    virtual void EnterState() override;
    virtual void OnState() override;
    virtual void EndState() override;

    //상태 변경을 요청한 State의 번호
    //상태 변경이 가능할 경우 true를 반환해 주면 상태를 변경시킬 수 있다.
    virtual bool CheckCondition(UINT _eState, tEvent _tEventMsg = tEvent{}) override;

    void Attack(CGameObject* _pTarget);
    //목적지를 CheckCondition으로 전달해주는 래핑함수
    void Move(Vec2 _v2Dest);

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
    //1당 32 픽셀
    UINT m_uWeaponRange;
    GETTER_SETTER(UINT, m_uWeaponRange, WeaponRange);

private:
    //State Check
    eSCUNIT_ATTACK_STATE m_eAttackState;
    bool m_bStopReserved;
    Vec2 m_v2ReservedDest; //예약된 이동 목적지

    //공격 대상
    CGameObject* m_pTarget;
    SETTER(CGameObject*, m_pTarget, Target);

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


inline void CScript_FSM_Attack::Move(Vec2 _v2Dest)
{
    Pack_v2_i64 pack(_v2Dest);
    Transition(FSM_SCUnit::MOVE, tEvent{ (DWORD_PTR)pack.i64, });
}
