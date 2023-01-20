#pragma once
#include "CSingleton.h"



class CLayer;
class CCollider2D;
class CGameObject;

struct tGrid2D
{
    vector<CCollider2D*> vecColliderInGrid;
};

union CollisionID
{
    struct
    {
        UINT32 LowID;
        UINT32 HighID;
    };
    UINT64 FullID;

    CollisionID(UINT32 _LowID, UINT32 _HighID)
        : LowID(_LowID)
        , HighID(_HighID)
    {}
};

struct tCollisionInfo
{
    bool bCurrent;
    CCollider2D* pColliderA;
    CCollider2D* pColliderB;

    tCollisionInfo(bool _bCurrent, CCollider2D* _pColliderA, CCollider2D* _pColliderB)
        : bCurrent(_bCurrent)
        , pColliderA(_pColliderA)
        , pColliderB(_pColliderB)
    {}
};


class CCollisionMgr :
    public CSingleton<CCollisionMgr>
{
private:
    vector<tGrid2D> m_vec2DGrid;
    UINT            m_uiNum2DGridX;
    UINT            m_uiNum2DGridY;
    UINT            m_uiNum2DGridTotalIndex;

    Vec2            m_v2DSpaceLB;
    Vec4            m_v4_2DSpaceLB;
    Vec2            m_v2DSpaceSize;

    Vec2            m_v2DGridSize;
    Vec4            m_v4_2DGridSizeInv;
    //참고 - Vec4는 편의를 위해 사용 중.
    //2DGridSizeInv의 경우 나눗셈을 미리 해두어서 곱셈을 하기 위한 용도의 변수임.

    UINT32          m_arrFlagLayerInteraction[MAX_LAYER];

    //Value로 저장된 bool 값 = 
    unordered_map<UINT64, tCollisionInfo>   m_umapCollisionID;

public:
    //void AddLayerInteract2D(UINT _iLowLayer, UINT _iHighLayer);
    //bool GetLayerInteract2D(UINT _iLowLayer, UINT _iHighLayer);
    //unordered_map<UINT64, bool>::iterator FindCollData(UINT32 _uID1, UINT32 _uID2);
    //void AddCollData2D(UINT32 _uID1, UINT32 _uID2, bool _bCollided);

    void AddCollider2D(CCollider2D* _pCol, tRectLBRT _AABBInfo);
    void AddCollider3D() {};

    void AddLayerInteraction2D(int _iLayer1, int _iLayer2);



public:
    //CollisionMgr의 호출 시점: CLevelMgr::tick()이 호출되는 시점. -> 가져다 써도 문제 없음.
    void Create2DGrid(Vec2 _vWorldLB, Vec2 _vWorldSize, UINT _uiGridNumX, UINT _uiGridNumY);

    //충돌 검사 시행
    void tick();

private:

    SINGLETON(CCollisionMgr)
};


inline void CCollisionMgr::AddLayerInteraction2D(int _iLayer1, int _iLayer2)
{
    m_arrFlagLayerInteraction[_iLayer1] |= 1 << _iLayer2;
    m_arrFlagLayerInteraction[_iLayer2] |= 1 << _iLayer1;
}

