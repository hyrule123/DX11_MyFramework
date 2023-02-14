#pragma once
#include "CCollider.h"

class CCollider2D_Rect;
class CCollider2D_Circle;
class CCollider2D_OBB;
class CCollider2D_Point;


//�⺻ : Rect �˻縦 ����
//�߰����� ���� �浹ü�� �ʿ��� ��� ��� Ŭ�������� ����ϸ� ��.
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
    //�������ҿ��� �˻�� �ڽ��� ���� �׸��� �ε��� ��ȣ
    vector<UINT>           m_vecGridIdxInfo;
    

public:
    tRectInfo       GetSpatialPartitionInfo()       const    { return m_SpatialPartitionInfo; }
    void            SetSpatialPartitionInfo(const tRectInfo& _Info) { m_SpatialPartitionInfo = _Info; }

    vector<UINT>& GetvecGridIdx() { return m_vecGridIdxInfo; }
    void SetvecGridIdx(vector<UINT>& _NewGridInfo) { return m_vecGridIdxInfo.swap(_NewGridInfo); }

public:
    virtual void finaltick() final;
    void UpdateCollider();

    virtual void DebugRender() = 0;
    virtual void cleanup() override {};
};

