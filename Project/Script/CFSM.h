#pragma once

#include <Engine/CScript.h>

//같은 FSM 범주 안의 클래스들은 enum class를 공유하는 헤더파일을 만들어서 사용해 줄것.
class CFSM
    : public CScript
{
public:
    CFSM(std::type_index _typeIdx, UINT _eMyState);
    virtual ~CFSM();

public:
    virtual void EnterState() = 0;
    virtual void OnState() = 0;
    virtual void EndState() = 0;

    //상태 변경을 요청한 State의 번호
    //상태 변경이 가능할 경우 true를 반환해 주면 상태를 변경시킬 수 있다.
    virtual bool CheckCondition(UINT _eState) = 0;
private:
    const UINT m_eMyState;

public:
    

};
