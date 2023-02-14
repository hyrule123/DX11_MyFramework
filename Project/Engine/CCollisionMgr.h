#pragma once
#include "CSingleton.h"

//**********************************************
// 충돌체가 그리드 하나 크기보다 커질 경우 에러 나므로 주의할 것
// 최대 4그리드까지만 점슈할 수 있도록 구현해 놓았음
//**********************************************


class CLayer;
class CGameObject;
class CCollider2D;
class CCollider2D_Rect;
class CCollider2D_Circle;
class CCollider2D_OBB;
class CCollider2D_Point;

class CCollider3D;

struct tGrid2D
{
    vector<CCollider2D*> vecColl;
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
    int            m_iNum2DGridX;
    int            m_iNum2DGridY;
    int            m_iNum2DGridTotalIndex;

    Vec2            m_v2DSpaceLB;
    Vec2            m_v2DSpaceSize;

    Vec2            m_v2DGridSize;

    Vec2            m_v2DGridSizeInv;//곱셈최적화용 변수
    //참고 - Vec4는 편의를 위해 사용 중.
    //2DGridSizeInv의 경우 나눗셈을 미리 해두어서 곱셈을 하기 위한 용도의 변수임.

    UINT32          m_arrFlagLayerInteraction[MAX_LAYER];

    
    unordered_map<UINT64, tCollisionInfo, tColmapHashFunc>   m_umapCollisionID;

public:
    //공간분할을 위한 자신의 간이 충돌체 꼭지점 정보를 전달한다.
    //Rect는 4개, Point는 1개 등 다를 수 있기 때문에 정점 위치를 vector에 담아서 전달하는 방식으로 구현
    //
    void AddCollider2D(CCollider2D* _pCol, const vector<Vec2>& _vecPoint);

    //이건 트랜스폼에서 추가 연산이 없었을 경우(자신의 간이정점 위치에 변함이 없을 경우) 호출
    void AddCollider2D(CCollider2D* _pCol, const vector<UINT>& _vecIdx);
    void AddCollider3D(CCollider3D* _pCol) {};
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


