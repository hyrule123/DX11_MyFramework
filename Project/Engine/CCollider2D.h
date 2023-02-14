#pragma once
#include "CCollider.h"

class CCollider2D_Rect;
class CCollider2D_Circle;
class CCollider2D_OBB;
class CCollider2D_Point;


//기본 : Rect 검사를 진행
//추가적인 정밀 충돌체가 필요할 경우 상속 클래스들을 사용하면 됨.
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
    //공간분할에서 검사된 자신이 속한 그리드 인덱스 번호
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

