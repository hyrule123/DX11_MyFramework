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
    //�켱 �������θ� �浹������ ����. ���߿� �ʿ������� ���� �����.
    vector<tGrid2D> m_vec2DGrid;
    int            m_iNum2DGridX;
    int            m_iNum2DGridY;
    int            m_iNum2DGridTotalIndex;

    Vec2            m_v2DSpaceLB;
    Vec2            m_v2DSpaceSize;

    Vec2            m_v2DGridSize;

    Vec2            m_v2DGridSizeInv;//��������ȭ�� ����
    //���� - Vec4�� ���Ǹ� ���� ��� ��.
    //2DGridSizeInv�� ��� �������� �̸� �صξ ������ �ϱ� ���� �뵵�� ������.

    UINT32          m_arrFlagLayerInteraction[MAX_LAYER];

    
    unordered_map<UINT64, tCollisionInfo, tColmapHashFunc>   m_umapCollisionID;

public:
    //���������� ���� �ڽ��� ���� �浹ü ������ ������ �����Ѵ�.
    //Rect�� 4��, Point�� 1�� �� �ٸ� �� �ֱ� ������ ���� ��ġ�� vector�� ��Ƽ� �����ϴ� ������� ����
    //
    void AddCollider2D(CCollider2D* _pCol, const vector<Vec2>& _vecPoint);

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
    //�浹ü Ÿ���� �з��ؼ� �Ʒ��� �Լ����� ȣ���Ѵ�.
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


