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
    CCollider3D(eCOLLIDER_TYPE_3D _eColType);
    virtual ~CCollider3D();

private:
    //공간분할 때 사용할 사각형 정보
    eCOLLIDER_TYPE_3D m_eColType;

public:
    eCOLLIDER_TYPE_3D  GetColliderType()   const { return m_eColType; }

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

