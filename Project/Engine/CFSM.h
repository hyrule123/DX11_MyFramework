#pragma once

#include "CScript.h"

#include "CFSM_Mgr.h"

class CGameObject;
class CTransform;
class CRenderComponent;
class CAnimator2D;
class CTilemapComplete;

//같은 FSM 범주 안의 클래스들은 enum class를 공유하는 헤더파일을 만들어서 사용해 줄것.
class CFSM_Mgr;
class CFSM
    : public CEntity
{
private:
    CFSM() = delete;
public:
    CFSM(UINT _eMyState);

    CFSM(const CFSM& _other);
    virtual CFSM* Clone() = 0;

    virtual ~CFSM();
    
public:
    virtual void init() {};

    virtual void EnterState() = 0;
    virtual void OnState() = 0;
    virtual void EndState() = 0;

    //상태 변경을 요청한 State의 번호
    //상태 변경이 가능할 경우 true를 반환해 주면 상태를 변경시킬 수 있다.
    virtual bool CheckCondition(UINT _eState) = 0;

    CFSM* Transition(UINT _eState);

public:
    virtual void BeginColiision(CCollider* _Other, const Vec3& _v3HitPoint) {};
    virtual void OnCollision(CCollider* _Other, const Vec3& _v3HitPoint) {};
    virtual void EndCollision(CCollider* _Other) {};

private:
    const UINT m_eMyState;
    CFSM_Mgr* m_pFStateMgr;

public:
    UINT GetMyState() const { return m_eMyState; }

    GETSET(CFSM_Mgr*, m_pFStateMgr, FStateMgr);

    CTransform* Transform() const { return m_pFStateMgr->Transform(); }
    CAnimator2D* Animator2D() const { return m_pFStateMgr->Animator2D(); }
    CScriptHolder* ScriptHolder() const { return m_pFStateMgr->ScriptHolder(); }


};
