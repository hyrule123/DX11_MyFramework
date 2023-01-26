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
    //�������� �� ����� �簢�� ����
    tRectInfo           m_SpatialPartitionInfo;

public:
    tRectInfo       GetSpatialPartitionInfo()       const    { return m_SpatialPartitionInfo; }
    void SetSpatialPartitionInfo(const tRectInfo& _SquareInfo) { m_SpatialPartitionInfo = _SquareInfo; }

public://�ڽŰ� ������ �浹�� üũ�ϴ� �޼ҵ�. ������ ID�� ���� �޼ҵ� �ʿ��� ȣ����.
    virtual bool CheckCollision(CCollider* _other) override;

private:
    virtual bool CheckCollisionRect(CCollider2D_Rect* _other) = 0;
    virtual bool CheckCollisionCircle(CCollider2D_Circle* _other) = 0;
    virtual bool CheckCollisionOBB2D(CCollider2D_OBB* _other) = 0;


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

