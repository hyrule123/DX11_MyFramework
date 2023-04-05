#pragma once

#include "CScript.h"

#include "CFStateMgr.h"

class CGameObject;
class CTransform;
class CRenderComponent;
class CAnimator2D;
class CTilemapComplete;

//같은 FSM 범주 안의 클래스들은 enum class를 공유하는 헤더파일을 만들어서 사용해 줄것.
class CFStateMgr;
class CFState
    : public CEntity
{
public:
    CFState(UINT _eMyState);

    CFState(const CFState& _other);
    virtual CFState* Clone() = 0;

    virtual ~CFState();
    
public:
    virtual void init() {};

    virtual void EnterState() = 0;
    virtual void OnState() = 0;
    virtual void EndState() = 0;

    //상태 변경을 요청한 State의 번호
    //상태 변경이 가능할 경우 true를 반환해 주면 상태를 변경시킬 수 있다.
    virtual bool CheckCondition(UINT _eState) = 0;

    CFState* Transition(UINT _eState);

public:
    virtual void BeginColiision(CCollider* _Other, const Vec3& _v3HitPoint) {};
    virtual void OnCollision(CCollider* _Other, const Vec3& _v3HitPoint) {};
    virtual void EndCollision(CCollider* _Other) {};

private:
    const UINT m_eMyState;
    CFStateMgr* m_pFStateMgr;
    CGameObject* m_pOwner;

public:
    UINT GetMyState() const { return m_eMyState; }

    GETSET(CFStateMgr*, m_pFStateMgr, FStateMgr);

    GETSET(CGameObject*, m_pOwner, OwnerObj);

    CTransform* Transform() const { return m_pOwner->Transform(); }
    CAnimator2D* Animator2D() const { return m_pOwner->Animator2D(); }
    CScriptHolder* ScriptHolder() const { return m_pOwner->ScriptHolder(); }


};
