#pragma once
#include "cCollider.h"

class cCollider2D_Rect;
class cCollider2D_Circle;
class cCollider2D_OBB;

class cCollider3D :
    public cCollider
{
private:
    cCollider3D() = delete;
public:
    cCollider3D(eCOLLIDER_TYPE_3D _eColType);
    virtual ~cCollider3D();

private:
    //공간분할 때 사용할 사각형 정보
    eCOLLIDER_TYPE_3D m_eColType;

public:
    eCOLLIDER_TYPE_3D  GetcColliderType()   const { return m_eColType; }

private:


    //public://충돌 시 호출할 함수
    //    virtual void BeginCollision(cCollider* _other);
    //    virtual void OnCollision(cCollider* _other);
    //    virtual void EndCollision(cCollider* _other);

public:
    virtual void finaltick() final;

    //자신의 충돌체 정보 + 간이 충돌체 정보를 업데이트 해줄 것
    virtual void UpdatecColliderInfo() = 0;
    virtual void DebugRender() = 0;
    virtual void cleanup() override {};
};

