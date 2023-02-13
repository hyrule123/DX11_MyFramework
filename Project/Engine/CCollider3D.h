#pragma once
#include "CCollider.h"

class CCollider2D_Rect;
class CCollider2D_Circle;
class CCollider2D_OBB;

class CCollider3D :
    public CCollider
{
private:
    CCollider3D() = delete;
public:
    CCollider3D(eCOLLIDER_TYPE _eColType);
    CCollider3D(const CCollider3D& _other);
    virtual ~CCollider3D();

private:
    //공간분할 때 사용할 사각형 정보

public:

public://자신과 상대방의 충돌을 체크하는 메소드. 무조건 ID가 작은 메소드 쪽에서 호출함.
    virtual bool CheckCollision(CCollider* _other) override;

private:


    //public://충돌 시 호출할 함수
    //    virtual void BeginCollision(CCollider* _other);
    //    virtual void OnCollision(CCollider* _other);
    //    virtual void EndCollision(CCollider* _other);

public:
    virtual void finaltick() final;

    //자신의 충돌체 정보 + 간이 충돌체 정보를 업데이트 해줄 것
    virtual void UpdateColliderInfo() = 0;
    virtual void DebugRender() = 0;
    virtual void cleanup() override {};
};

