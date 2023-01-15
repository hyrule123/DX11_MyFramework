#pragma once
#include "CSingleton.h"

#include "CSpatialPartition.h"

union CollisionID
{
    struct
    {
        UINT32 LowID;  //충돌체 중 ID 숫자가 작은 충돌체의 ID
        UINT32 HighID; //충돌체 중 ID 숫자가 큰 충돌체의 ID
    };

    UINT64 FullID;
};

class CLayer;
class CCollider2D;
class CGameObject;

class CCollisionMgr :
    public CSingleton<CCollisionMgr>
{
private:
    //2D 공간분할 충돌검사 매니저
    CSpatialPartition m_SpatialPartition2D;
    CSpatialPartition m_SpatialPartition3D;

public:
    //void AddLayerInteract2D(UINT _iLowLayer, UINT _iHighLayer);
    //bool GetLayerInteract2D(UINT _iLowLayer, UINT _iHighLayer);
    //unordered_map<UINT64, bool>::iterator FindCollData(UINT32 _uID1, UINT32 _uID2);
    //void AddCollData2D(UINT32 _uID1, UINT32 _uID2, bool _bCollided);

    void AddCollider2D(CCollider2D* _pCol, const Vec2& _vLBPos, const Vec2& _vColSize);
    void AddCollider3D() {};


private:
    void CollisionBtwLayer(int _iLayerNumLow, int _iLayerNumHigh);
    void CollisionBtwObject(CGameObject* _pLeft, CGameObject* _pRight);
    void CollisionBtwCollider(CCollider2D* _pLeft, CCollider2D* _pRight);


public:
    //CollisionMgr의 호출 시점: CLevelMgr::tick()이 호출되는 시점. -> 가져다 써도 문제 없음.
    void init();
    void tick();

private:


    SINGLETON(CCollisionMgr)
};



