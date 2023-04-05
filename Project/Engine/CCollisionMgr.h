#pragma once

#include "define.h"
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




class CCollisionMgr
    : public CSingleton<CCollisionMgr>
{
    SINGLETON(CCollisionMgr);
       
private:
    //우선 논리적으로만 충돌정보를 저장. 나중에 필요해지면 따로 만들것.
    vector<tGrid2D> m_vec2DGrid;
    int            m_iNum2DGridX;
    int            m_iNum2DGridY;
    int            m_iNum2DGridTotalIndex;

    Vec2            m_vSpaceLBPos;
    Vec4            m_v4SpaceLBPos;

    Vec2            m_vSpaceSize;

    Vec2            m_vGridSize;

    Vec2            m_vGridSizeInv;//곱셈최적화용 변수
    Vec4            m_v4GridSizeInv;

    //참고 - Vec4는 편의를 위해 사용 중.
    //2DGridSizeInv의 경우 나눗셈을 미리 해두어서 곱셈을 하기 위한 용도의 변수임.
    UINT32          m_arrFlagLayerInteraction[MAX_LAYER];

    
    unordered_map<UINT64, tCollisionInfo, tLightHashFunc_UINT64>   m_umapCollisionID;

    std::function<bool(CCollider2D*, CCollider2D*, Vec2&)> m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::END][(int)eCOLLIDER_TYPE_2D::END];
public:
    //공간분할을 위한 자신의 간이 충돌체 꼭지점 정보를 전달한다.
    //Rect는 4개, Point는 1개 등 다를 수 있기 때문에 정점 위치를 vector에 담아서 전달하는 방식으로 구현
    //
    void CalcSimpleCollGrid2D(__in CCollider2D* _pCol, __in Vec4 _vLBRTPos, __out vector<UINT>& _vecIdx);

    //이건 트랜스폼에서 추가 연산이 없었을 경우(자신의 간이정점 위치에 변함이 없을 경우) 호출
    void AddCollider2D(CCollider2D* _pCol, const vector<UINT>& _vecIdx);
    void AddCollider3D(CCollider3D* _pCol) {};
    void AddLayerInteraction2D(int _iLayer1, int _iLayer2);

    //지정한 레이어와 모든 레이어와 충돌 상호작용하도록 변경한다.
    void AddLayerInterAction2DAll(int _iLayer);

public:
    //CollisionMgr의 호출 시점: CLevelMgr::tick()이 호출되는 시점. -> 가져다 써도 문제 없음.
    void Create2DGrid(Vec2 _vWorldLB, Vec2 _vWorldSize, UINT _uiGridNumX, UINT _uiGridNumY);

    //충돌 검사 시행
    void tick();


private://충돌 검사 함수

    //충돌함수를 함수포인터에 등록해주는 함수
    void RegisterCollisionFunc();


    //충돌체 타입을 분류해서 아래의 함수들을 호출한다.
    bool CheckCollision2D(CCollider2D* _pCol_1, CCollider2D* _pCol_2, Vec2& _v2HitPoint);

    bool CheckCollision2D_Rect_Rect(CCollider2D* _pColRect_1, CCollider2D* _pColRect_2, Vec2& _v2HitPoint);
    bool CheckCollision2D_Rect_Circle(CCollider2D* _pColRect, CCollider2D* _pColCircle, Vec2& _v2HitPoint);
    bool CheckCollision2D_Rect_OBB(CCollider2D* _pColRect, CCollider2D* _pColOBB, Vec2& _v2HitPoint);

    bool CheckCollision2D_Rect_Point(CCollider2D* _pColRect, CCollider2D* _pColPoint, Vec2& _v2HitPoint);
    bool CheckCollision2D_Rect_Point_CollInfo(const Vec4& _v4LBRT, const Vec2& _v2Point);

   
    bool CheckCollision2D_Circle_Circle(CCollider2D* _pColCircle_1, CCollider2D* _pColCircle_2, Vec2& _v2HitPoint);
    bool CheckCollision2D_Circle_OBB(CCollider2D* _pColCircle, CCollider2D* _pColOBB, Vec2& _v2HitPoint);

    bool CheckCollision2D_Circle_Point(CCollider2D* _pColCircle, CCollider2D* _pColPoint, Vec2& _v2HitPoint);
    bool CheckCollision2D_Circle_Point_CollInfo(const Vec2& _v2CircleCenterPos, float _fCircleRadius, const Vec2& _v2PointPos);


    bool CheckCollision2D_OBB_OBB(CCollider2D* _pColOBB2D_1, CCollider2D* _pColOBB2D_2, Vec2& _v2HitPoint);
    bool CheckCollision2D_OBB_Point(CCollider2D* _pColOBB2D, CCollider2D* _pColPoint, Vec2& _v2HitPoint);

    bool CheckCollision2D_Point_Point(CCollider2D* _pColPoint_1, CCollider2D* _pColPoint_2, Vec2& _v2HitPoint);

private:
    //6   7    8
    //  =====
    //3 | 4 |  5
    //  =====
    //0   1    2    
    //점이 사각형의 어느 지점에 있는지를 인덱스 형태로 반환한다(위 도식 참고)
    UINT ComputeRelativePos_Rect_Point(const Vec4& _v4RectLBRT, const Vec2& _v2Point);

    //LBRT 정보가 들어있는 사각형을 통해 겹치는 부분을 구한다.
    //일반적으로 정확도를 조금 희생하고 Simple Collider를 사용
    void ComputeHitPointByRectLBRT(const Vec4& _v4LBRT_1, const Vec4& _v4LBRT_2, Vec2& _v2HitPoint);
};


inline void CCollisionMgr::AddLayerInteraction2D(int _iLayer1, int _iLayer2)
{
    m_arrFlagLayerInteraction[_iLayer1] |= 1u << _iLayer2;
    m_arrFlagLayerInteraction[_iLayer2] |= 1u << _iLayer1;
}

inline void CCollisionMgr::AddLayerInterAction2DAll(int _iLayer)
{
    m_arrFlagLayerInteraction[_iLayer] = UINT32_MAX;

    for (int i = 0; i < MAX_LAYER; ++i)
    {
        m_arrFlagLayerInteraction[i] |= 1u << _iLayer;
    }
}


