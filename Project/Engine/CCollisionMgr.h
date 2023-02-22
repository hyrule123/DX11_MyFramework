#pragma once
#include "CSingleton.h"

//**********************************************
// �浹ü�� �׸��� �ϳ� ũ�⺸�� Ŀ�� ��� ���� ���Ƿ� ������ ��
// �ִ� 4�׸�������� ������ �� �ֵ��� ������ ������
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
    LONGLONG llCheckedCount; //CTimeMgr���� quadpart�� �״�� �޾ƿͼ� üũ�� �ð��� ����Ѵ�.
    CCollider2D* pColliderA;
    CCollider2D* pColliderB;
};




class CCollisionMgr :
    public CSingleton<CCollisionMgr>
{
    SINGLETON(CCollisionMgr)

private:
    //�켱 �������θ� �浹������ ����. ���߿� �ʿ������� ���� �����.
    vector<tGrid2D> m_vec2DGrid;
    int            m_iNum2DGridX;
    int            m_iNum2DGridY;
    int            m_iNum2DGridTotalIndex;

    Vec2            m_vSpaceLBPos;
    Vec4            m_v4SpaceLBPos;

    Vec2            m_vSpaceSize;

    Vec2            m_vGridSize;

    Vec2            m_vGridSizeInv;//��������ȭ�� ����
    Vec4            m_v4GridSizeInv;

    //���� - Vec4�� ���Ǹ� ���� ��� ��.
    //2DGridSizeInv�� ��� �������� �̸� �صξ ������ �ϱ� ���� �뵵�� ������.
    UINT32          m_arrFlagLayerInteraction[MAX_LAYER];

    
    unordered_map<UINT64, tCollisionInfo, tLightHashFunc_UINT64>   m_umapCollisionID;

    std::function<bool(CCollider2D*, CCollider2D*)> m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::END][(int)eCOLLIDER_TYPE_2D::END];
public:
    //���������� ���� �ڽ��� ���� �浹ü ������ ������ �����Ѵ�.
    //Rect�� 4��, Point�� 1�� �� �ٸ� �� �ֱ� ������ ���� ��ġ�� vector�� ��Ƽ� �����ϴ� ������� ����
    //
    void CalcAndAddCollider2D(__in CCollider2D* _pCol, __in Vec4 _vLBRTPos, __out vector<UINT>& _vecIdx);

    //�̰� Ʈ���������� �߰� ������ ������ ���(�ڽ��� �������� ��ġ�� ������ ���� ���) ȣ��
    void AddCollider2D(CCollider2D* _pCol, const vector<UINT>& _vecIdx);
    void AddCollider3D(CCollider3D* _pCol) {};
    void AddLayerInteraction2D(int _iLayer1, int _iLayer2);

public:
    //CollisionMgr�� ȣ�� ����: CLevelMgr::tick()�� ȣ��Ǵ� ����. -> ������ �ᵵ ���� ����.
    void Create2DGrid(Vec2 _vWorldLB, Vec2 _vWorldSize, UINT _uiGridNumX, UINT _uiGridNumY);

    //�浹 �˻� ����
    void tick();


private://�浹 �˻� �Լ�

    //�浹�Լ��� �Լ������Ϳ� ������ִ� �Լ�
    void RegisterCollisionFunc();


    //�浹ü Ÿ���� �з��ؼ� �Ʒ��� �Լ����� ȣ���Ѵ�.
    bool CheckCollision2D(CCollider2D* _pCol_1, CCollider2D* _pCol_2);

    bool CheckCollision2D_Rect_Rect(CCollider2D* _pColRect_1, CCollider2D* _pColRect_2);
    bool CheckCollision2D_Rect_Circle(CCollider2D* _pColRect, CCollider2D* _pColCircle);
    bool CheckCollision2D_Rect_OBB(CCollider2D* _pColRect, CCollider2D* _pColOBB);
    bool CheckCollision2D_Rect_Point(CCollider2D* _pColRect, CCollider2D* _pColPoint);

   
    bool CheckCollision2D_Circle_Circle(CCollider2D* _pColCircle_1, CCollider2D* _pColCircle_2);
    bool CheckCollision2D_Circle_OBB(CCollider2D* _pColCircle, CCollider2D* _pColOBB);
    bool CheckCollision2D_Circle_Point(CCollider2D* _pColCircle, CCollider2D* _pColPoint);


    bool CheckCollision2D_OBB_OBB(CCollider2D* _pColOBB2D_1, CCollider2D* _pColOBB2D_2);
    bool CheckCollision2D_OBB_Point(CCollider2D* _pColOBB2D, CCollider2D* _pColPoint);

    bool CheckCollision2D_Point_Point(CCollider2D* _pColPoint_1, CCollider2D* _pColPoint_2);
    
    
};


inline void CCollisionMgr::AddLayerInteraction2D(int _iLayer1, int _iLayer2)
{
    m_arrFlagLayerInteraction[_iLayer1] |= 1 << _iLayer2;
    m_arrFlagLayerInteraction[_iLayer2] |= 1 << _iLayer1;
}


