#pragma once
#include "ICollider.h"

class cCom_Coll2D_Rect;
class cCom_Coll2D_Circle;
class cCom_Coll2D_OBB;

class ICollider3D :
    public ICollider
{
private:
    ICollider3D() = delete;
public:
    ICollider3D(eCOLLIDER_TYPE_3D _eColType);
    virtual ~ICollider3D();

private:
    //공간분할 때 사용할 사각형 정보
    eCOLLIDER_TYPE_3D m_eColType;

public:
    eCOLLIDER_TYPE_3D  GetcColliderType()   const { return m_eColType; }

private:


    //public://충돌 시 호출할 함수
    //    virtual void BeginCollision(ICollider* _other);
    //    virtual void OnCollision(ICollider* _other);
    //    virtual void EndCollision(ICollider* _other);

public:
    virtual void finaltick() final;

    //자신의 충돌체 정보 + 간이 충돌체 정보를 업데이트 해줄 것
    virtual void UpdatecColliderInfo() = 0;
    virtual void DebugRender() = 0;
    virtual void cleanup() override {};
};

