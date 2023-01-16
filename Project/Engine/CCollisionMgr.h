#pragma once
#include "CSingleton.h"



class CLayer;
class CCollider2D;
class CGameObject;
class CSpatialPartition2D;
class CSpatialPartition3D;

class CCollisionMgr :
    public CSingleton<CCollisionMgr>
{
private:

    //2D 공간분할 충돌검사 매니저
    CSpatialPartition2D* m_SpatialPartition2D;
    CSpatialPartition3D* m_SpatialPartition3D;

public:
    //void AddLayerInteract2D(UINT _iLowLayer, UINT _iHighLayer);
    //bool GetLayerInteract2D(UINT _iLowLayer, UINT _iHighLayer);
    //unordered_map<UINT64, bool>::iterator FindCollData(UINT32 _uID1, UINT32 _uID2);
    //void AddCollData2D(UINT32 _uID1, UINT32 _uID2, bool _bCollided);

    void AddCollider2D(CCollider2D* _pCol);
    void AddCollider3D() {};

    void AddLayerInteraction2D(int _iLayer1, int _iLayer2);



public:
    //CollisionMgr의 호출 시점: CLevelMgr::tick()이 호출되는 시점. -> 가져다 써도 문제 없음.
    void init();
    void tick();

private:


    SINGLETON(CCollisionMgr)
};



