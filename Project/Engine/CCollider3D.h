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
    //�������� �� ����� �簢�� ����
    eCOLLIDER_TYPE_3D m_eColType;

public:
    eCOLLIDER_TYPE_3D  GetColliderType()   const { return m_eColType; }

private:


    //public://�浹 �� ȣ���� �Լ�
    //    virtual void BeginCollision(CCollider* _other);
    //    virtual void OnCollision(CCollider* _other);
    //    virtual void EndCollision(CCollider* _other);

public:
    virtual void finaltick() final;

    //�ڽ��� �浹ü ���� + ���� �浹ü ������ ������Ʈ ���� ��
    virtual void UpdateColliderInfo() = 0;
    virtual void DebugRender() = 0;
    virtual void cleanup() override {};
};

