#pragma once
#include "CComponent.h"

class CCollider :
    public CComponent
{
private:
    CCollider() = delete;
public:
    CCollider(eCOMPONENT_TYPE _ComType);
    virtual ~CCollider();

private:

    Matrix              m_matCollider;      // Collider �� �������

    Vec3                m_vOffsetPos;
    Vec3                m_vOffsetScale;
    bool                m_bFixSize;         // ���� ����� ���
    int                 m_iCollisionCount;  // ���� �浹���� �浹ü�� ������ ����

    bool                m_bNeedPosUpdate;
    bool                m_bNeedRotUpdate;
    bool                m_bNeedScaleSizeUpdate;




public:
    //inline Getter
    
    const Vec3&            GetOffsetPos()      const { return m_vOffsetPos; }
    const Vec3&            GetOffsetScale()    const { return m_vOffsetScale; }

    void            AddCollisionCount() { ++m_iCollisionCount; }
    void            SubCollisionCount() { --m_iCollisionCount; }
    int             GetCollisionCount() const { return m_iCollisionCount; }

    int             GetLayerIndex()           { return GetOwner()->GetLayer(); }
    

    //nullptr�� ��ȯ�� �� �����Ƿ� �����Ұ�
    CScriptHolder* ScriptHolder()             { return GetOwner()->ScriptHolder(); }

    //inline Setter
    
    void SetColliderMatrix(const Matrix& _mat) { m_matCollider = _mat; }


public:
    void BeginCollision(CCollider* _other);
    void OnCollision(CCollider* _other);
    void EndCollision(CCollider* _other);

public:
    //�浹ü finaltick()�� transform::finaltick()���� ȣ��ǹǷ� transform�� ���� ������ ������ ����ص� ���� ����.
    virtual void finaltick() = 0;

    //���� �浹ü�� ������ AABB �簢�� ����(m_RectInfo)�� ������Ʈ �ؾ���.
    virtual void UpdateCollider() = 0;
};