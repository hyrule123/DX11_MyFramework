#pragma once

#include "IScript.h"

class cGameObject;
class cCom_Transform;
class IRenderer;
class cCom_Animator2D;
class cScriptHolder;

struct tDescFSM
{
    const UINT uMyStateID;
    const UINT uMaxStateID;

    tDescFSM(UINT _uMyStateID, UINT _uMaxStateID) : uMyStateID(_uMyStateID), uMaxStateID(_uMaxStateID) {}
    tDescFSM(const tDescFSM& _other) = delete;
    tDescFSM(tDescFSM&& _right) = delete;
};


//같은 cFSM 범주 안의 클래스들은 enum class를 공유하는 헤더파일을 만들어서 사용해 줄것
class cFSM
    : public IScript
{
private:
    cFSM() = delete;
public:
    //strKey -> IScript로 전달, _uStateID -> cFSM으로 전달
    cFSM(const string_view _strKey, UINT _uStateID);

    cFSM(const cFSM& _other);
    virtual cFSM* Clone() = 0;

    virtual ~cFSM();
    
public:
    virtual void init() {};

    //상태 변경을 요청한 State의 번호
    //상태 변경이 가능할 경우 true를 반환해 주면 상태를 변경시킬 수 있다.
    virtual eFSM_RESULT CheckCondition(const tFSM_Event& _tEvent)= 0;

    //호출 시점: Transition()에서 true가 반환될 경우(상태 진입)
    virtual void EnterState(const tFSM_Event& _tEvent) = 0;

    //호출 시점: 자신이 현재 State일 경우, 스크립트의 tick() 순회 후 호출(상태 진행)
    virtual void OnState() = 0;

    //호출 시점: Transition()에서 true가 반환될 경우(상태 종료)
    virtual void EndState() = 0;


protected:
    eFSM_RESULT Transition(const tFSM_Event& _tEvent);

public:
    virtual void SetHolder(cScriptHolder* _pScriptHolder) override;

private:
    const UINT m_uStateID;
public:
    UINT GetStateID() const { return m_uStateID; }
};

inline eFSM_RESULT cFSM::Transition(const tFSM_Event& _tEvent)
{
    return ScriptHolder()->Transition(_tEvent);
}
