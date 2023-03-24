#pragma once

#include <Engine/CScript.h>

//FSM
enum class eSTATE
{
    IDLE,
    MOVE,
    ATTACK
};

class CScript_Player :
    public CScript
{
public:
    CScript_Player();
    //CScript_Player(const CScript_Player& _other); //기본 복사생성자로 충분함.
    ~CScript_Player();

    CLONE(CScript_Player);

public:
    virtual void init() override;
    virtual void tick() override;

public:
    virtual void OnCollision(CCollider* _pCol) override;

private:
    Vec4 m_ColorKey;
    float m_MoveSpeed;
    float m_TurningForceRad;
    eSTATE m_eState;

    //TODO : 마우스 클릭 확인을 위한 임시 변수
    bool m_bSelected;



private:
    void Shoot();
};

