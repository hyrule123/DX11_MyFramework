#pragma once
#include "CEntity.h"

#include "CSpatialGrid.h"

class CCollisionMgr;


class CSpatialPartition :
    public CEntity
{
private:
    CSpatialPartition() = delete;
public:
    CSpatialPartition(eDIMENSION_TYPE _type);
    virtual ~CSpatialPartition();
    CLONE_DISABLE(CSpatialPartition)

protected:
    //�迭�� �ε��� = �ڽ��� ���̾�
    //���� �ε��� = �ڽ��� ���̾�� �浹�� Ȯ���� ���̾�(��Ʈ �÷��� ���)
    UINT32  m_flagLayerCollision[MAX_LAYER];

    //�� �浹ü ���̿��� �浹�� �Ͼ���� ���θ� ����(�浹ü 2���� CEntity::m_iID�� �ٿ��� �����Ѵ�.)
    unordered_map<UINT64, bool> m_umapColData;


    //2D�� 3D�� �׸��� ���� ����� �ٸ��Ƿ� ��� ���·� ������ �ҵ�
    const eDIMENSION_TYPE   m_eDimensionType;

    float m_fGridSize;       //���� �׸����� ������

    float m_fGridLBFPos;     //���� �׸����� ���ϴ� ��ġ

    UINT  m_uiGridLevel;

    UINT  m_uiGridSideNum;      //��� �׸����� ���μ��� ����. ������ ¦�� ������ ����(2, 4, 6, ...)
    UINT  m_uiGridSideSquare;   //��� �׸����� ����� ����(z�� ����)
    UINT  m_uiGridTotalNum;     //��� �׸����� ����


    //���� ������ ���� Ȱ��ȭ
    bool  m_bDynamicSizeOn;

    //�ٿ����¡�� ���� ����
    UINT  m_uiMaxGridIndex;
    bool  m_bTimeRewind;    //�ð��� �ʱ�ȭ�ؾ��ϴ��� ���� ���
    float m_fLevelDownTimeRemain;

    //������¡/�ٿ����¡�� ���� ����
    int  m_uiGridLevelChangeReserve;

    vector<CSpatialGrid>   m_vecGrid;

public:
    bool GetLayerInteract(UINT _iLayer1, UINT _iLayer2);


    //unordered_map<UINT64, bool>::iterator FindCollData(UINT32 _uID1, UINT32 _uID2);


    void SetGridSize(float _fGridSize) { m_fGridSize = _fGridSize; }

    //CCollider���� ȣ��. �ڽ��� ���ϴܰ� ���� ��ġ�� �����ϸ� �ش� ���� ���� ����� �׸��忡 �����ִ����� ����Ѵ�.
    void ComputeGridArea(CCollider* _pCol, const Vec3& _vColLeftBottomFront, const Vec3& _vColSize);

private:
    UINT Abs(int _i) { if (_i < 0) return  (UINT)-_i; return (UINT)_i; }
    void Sort(UINT32& _uLow, UINT32& _uHigh);
    void ReserveLevelUp(int _uiLevel);
    void ChangeGridLevel(int _uiLevelChange);

public:
    virtual void init();
    void tick();
    void CheckCollision();
};


inline void CSpatialPartition::Sort(UINT32& _uLow, UINT32& _uHigh)
{
    if (_uLow <= _uHigh)
        return;

    UINT32 Temp = _uLow;
    _uLow = _uHigh;
    _uHigh = Temp;
}

inline void CSpatialPartition::ReserveLevelUp(int _uiLevel)
{
    if (m_uiGridLevelChangeReserve < _uiLevel)
        m_uiGridLevelChangeReserve = _uiLevel;
}

inline bool CSpatialPartition::GetLayerInteract(UINT _iLayer1, UINT _iLayer2)
{
    UINT uLow = _iLayer1;
    UINT uHigh = _iLayer2;
    Sort(uLow, uHigh);

    return (m_flagLayerCollision[uLow] & (1 << uHigh));
}
