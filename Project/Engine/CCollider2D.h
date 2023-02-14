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

    //공간분할에서 검사할 자신의 꼭짓점 정보(형태에 따라 꼭짓점의 갯수가 달라질 수 있으므로)
    vector<Vec2>           m_vecSpatialPartitionVtx;
    
    //간이충돌체는 각자 충돌체에서 따로 구현해줄것.(점충돌체 때문에)

    //오프셋 위치를 반영한 위치(2D)
    Vec2                   m_vCenterPos;

public:
    void SetvecGridIdx(vector<UINT>& _vecIdx) { std::swap(m_vecGridIdxInfo, _vecIdx); }
    vector<UINT>& GetvecGridIdx() { return m_vecGridIdxInfo; }

    Vec2 GetCenterPos() const { return m_vCenterPos; }

public:
    virtual void finaltick() final;
    virtual void UpdateCollider() = 0;
    
    //인자로 들어가는 변수에 넣으면 됨
    virtual void UpdateAABBInfo() = 0;
    virtual void UpdateSpatialPartitionInfo(vector<Vec2>& _vecSpatialPartitionVtx) = 0;
    virtual void DebugRender() = 0;
    virtual void cleanup() override {};
};

