#pragma once
#include "CComponent.h"

class CCollider :
    public CComponent
{
private:
    CCollider() = delete;
public:
    CCollider(eCOMPONENT_TYPE _ComType, eCOLLIDER_TYPE _ColType);
    virtual ~CCollider();

private:
    eCOLLIDER_TYPE      m_eColType;
    Matrix              m_matCollider;      // Collider �� �������

    Vec3                m_vOffsetPos;
    Vec3                m_vOffsetScale;
    bool                m_bFixSize;         // ���� ����� ���
    int                 m_iCollisionCount;  // ���� �浹���� �浹ü�� ������ ����

public:
    //inline Getter
    eCOLLIDER_TYPE  GetColliderType()   const { return m_eColType; }
    Vec3            GetOffsetPos()      const { return m_vOffsetPos; }
    Vec3            GetOffsetScale()    const { return m_vOffsetScale; }
    int             GetCollisionCount() const { return m_iCollisionCount; }

    //inline Setter
    void SetColliderMatrix(const Matrix& _mat) { m_matCollider = _mat; }


public:
    virtual bool CheckCollision(CCollider* _other) = 0;
    void BeginCollision(CCollider* _other);
    void OnCollision(CCollider* _other);
    void EndCollision(CCollider* _other);

public:
    //�浹ü finaltick()�� transform::finaltick()���� ȣ��ǹǷ� transform�� ���� ������ ������ ����ص� ���� ����.
    virtual void finaltick();
};

