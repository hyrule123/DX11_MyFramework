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
    CCollider2D(eCOLLIDER_TYPE_2D _eColType);
    virtual ~CCollider2D();

private:
    eCOLLIDER_TYPE_2D      m_eColType;

    //�������ҿ��� �˻�� �ڽ��� ���� �׸��� �ε��� ��ȣ
    vector<UINT>           m_vecGridIdxInfo;

    //�������ҿ� ���� �浹ü ����(���簢�� ���� - �⺻ ������ transform���� ���簢�� ����)
    //�⺻ ��
    Vec4                   m_vSimpleCollLBRTPos;
    
    //�����浹ü�� ���� �浹ü���� ���� �������ٰ�.(���浹ü ������)

    //������ ��ġ�� �ݿ��� ��ġ(2D)
    Vec2                   m_vCenterPos;

    
    

public:
    eCOLLIDER_TYPE_2D  GetColliderType()   const { return m_eColType; }
    void SetvecGridIdx(vector<UINT>& _vecIdx) { std::swap(m_vecGridIdxInfo, _vecIdx); }

    Vec2 GetCenterPos() const { return m_vCenterPos; }

public:
    virtual void finaltick() final;
    virtual void UpdateCollider() = 0;
    
    //�⺻ �������� transform���� �޾ƿ� ���� ���̷� ���� �浹ü�� �����.
    //�� ������ �浹ü�� �̰� ������
    virtual void UpdateSimpleCollider(Vec4& _vSimpleCollLBRTPos);
    virtual void DebugRender() = 0;
    virtual void cleanup() override {};
};

