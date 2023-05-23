#pragma once

#include "CScript.h"

class CGameObject;
class CTransform;
class CRenderComponent;
class CAnimator2D;
class CTilemapComplete;

//같은 FSM 범주 안의 클래스들은 enum class를 공유하는 헤더파일을 만들어서 사용해 줄것.
class CFSM_Mgr;
class CFSM
    : public CScript
{
private:
    CFSM() = delete;
public:
    CFSM(UINT _uStateID);

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
    virtual bool CheckCondition(UINT _eState, tEvent _tEventMsg) = 0;

    virtual void SetHolder(CScriptHolder* _pScriptHolder) override;

private:
    const UINT m_uStateID;
public:
    UINT GetStateID() const { return m_uStateID; }
};
