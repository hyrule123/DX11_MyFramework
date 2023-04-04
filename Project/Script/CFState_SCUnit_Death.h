#pragma once
#include <Engine/CFState.h>

class CFState_SCUnit_Death :
    public CFState
{
public:
    CFState_SCUnit_Death();
    CFState_SCUnit_Death(std::type_index _type);
    virtual ~CFState_SCUnit_Death();

public:
    virtual void EnterState() override;
    virtual void OnState() override;
    virtual void EndState() override;

    //상태 변경을 요청한 State의 번호
    //상태 변경이 가능할 경우 true를 반환해 주면 상태를 변경시킬 수 있다.
    virtual bool CheckCondition(UINT _eState) override;
};

