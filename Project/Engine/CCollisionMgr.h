#pragma once
#include "CSingleton.h"


class CLayer;
class CGameObject;
class CCollider2D;
class CCollider2D_Rect;
class CCollider2D_Circle;
class CCollider2D_OBB;
class CCollider2D_Point;




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
};

struct tCollisionInfo
{
    LONGLONG llCheckedCount; //CTimeMgr에서 quadpart를 그대로 받아와서 체크한 시간을 기록한다.
    CCollider2D* pColliderA;
    CCollider2D* pColliderB;
};

struct tColmapHashFunc
{
    UINT64 operator()(const UINT64& _ukey) const
    {
        return static_cast<UINT64>(_ukey);
    }
};


class CCollisionMgr :
    public CSingleton<CCollisionMgr>
{
    SINGLETON(CCollisionMgr)

private:
    //우선 논리적으로만 충돌정보를 저장. 나중에 필요해지면 따로 만들것.
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

    
    unordered_map<UINT64, tCollisionInfo, tColmapHashFunc>   m_umapCollisionID;

public:
    void AddCollider2D(CCollider2D* _pCol);
    void AddCollider3D() {};

    void AddLayerInteraction2D(int _iLayer1, int _iLayer2);

public:
    //CollisionMgr의 호출 시점: CLevelMgr::tick()이 호출되는 시점. -> 가져다 써도 문제 없음.
    void Create2DGrid(Vec2 _vWorldLB, Vec2 _vWorldSize, UINT _uiGridNumX, UINT _uiGridNumY);

    //충돌 검사 시행
    void tick();


private://충돌 검사 함수
    //충돌체 타입을 분류해서 아래의 함수들을 호출한다.
    bool CheckCollision2D(CCollider2D* _pCol_1, CCollider2D* _pCol_2);


    bool CheckCollision2D_Rect_Rect(CCollider2D_Rect* _pColRect_1, CCollider2D_Rect* _pColRect_2);
    bool CheckCollision2D_Circle_Circle(CCollider2D_Circle* _pColCircle_1, CCollider2D_Circle* _pColCircle_2);
    bool CheckCollision2D_OBB2D_OBB2D(CCollider2D_OBB* _pColOBB2D_1, CCollider2D_OBB* _pColOBB2D_2);
    bool CheckCollision2D_OBB2D_Point(CCollider2D_OBB* _pColOBB2D, CCollider2D_Point* _pColPoint);

    

};


inline void CCollisionMgr::AddLayerInteraction2D(int _iLayer1, int _iLayer2)
{
    m_arrFlagLayerInteraction[_iLayer1] |= 1 << _iLayer2;
    m_arrFlagLayerInteraction[_iLayer2] |= 1 << _iLayer1;
}


