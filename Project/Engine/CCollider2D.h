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

    //�������ҿ��� �˻��� �ڽ��� ������ ����(���¿� ���� �������� ������ �޶��� �� �����Ƿ�)
    vector<Vec2>           m_vecSpatialPartitionVtx;
    
    //�����浹ü�� ���� �浹ü���� ���� �������ٰ�.(���浹ü ������)

    //������ ��ġ�� �ݿ��� ��ġ(2D)
    Vec2                   m_vCenterPos;

public:
    void SetvecGridIdx(vector<UINT>& _vecIdx) { std::swap(m_vecGridIdxInfo, _vecIdx); }
    vector<UINT>& GetvecGridIdx() { return m_vecGridIdxInfo; }

    Vec2 GetCenterPos() const { return m_vCenterPos; }

public:
    virtual void finaltick() final;
    virtual void UpdateCollider() = 0;
    
    //���ڷ� ���� ������ ������ ��
    virtual void UpdateAABBInfo() = 0;
    virtual void UpdateSpatialPartitionInfo(vector<Vec2>& _vecSpatialPartitionVtx) = 0;
    virtual void DebugRender() = 0;
    virtual void cleanup() override {};
};

