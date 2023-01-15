#pragma once
#include "CCollider.h"

class CCollider2D_Rect;
class CCollider2D_Circle;
class CCollider2D_OBB;

class CCollider2D :
    public CCollider
{
private:
    CCollider2D() = delete;
public:
    CCollider2D(eCOLLIDER_TYPE _eColType);
    CCollider2D(const CCollider2D& _other);
    virtual ~CCollider2D();

private:

public://자신과 상대방의 충돌을 체크하는 메소드. 무조건 ID가 작은 메소드 쪽에서 호출함.
    virtual bool CheckCollision(CCollider* _other) override;

private:
    virtual bool CheckCollisionRect(CCollider2D_Rect* _other) = 0;
    virtual bool CheckCollisionCircle(CCollider2D_Circle* _other) = 0;
    virtual bool CheckCollisionOBB2D(CCollider2D_OBB* _other) = 0;


//public://충돌 시 호출할 함수
//    virtual void BeginCollision(CCollider* _other);
//    virtual void OnCollision(CCollider* _other);
//    virtual void EndCollision(CCollider* _other);

public:
};

