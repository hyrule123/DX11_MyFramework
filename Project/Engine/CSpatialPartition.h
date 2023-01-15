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
    //배열의 인덱스 = 자신의 레이어
    //내부 인덱스 = 자신의 레이어와 충돌을 확인한 레이어(비트 플래그 방식)
    UINT32  m_flagLayerCollision[MAX_LAYER];

    //두 충돌체 사이에서 충돌이 일어났는지 여부를 저장(충돌체 2개의 CEntity::m_iID를 붙여서 저장한다.)
    unordered_map<UINT64, bool> m_umapColData;


    //2D와 3D의 그리드 생성 방식이 다르므로 상속 형태로 만들어야 할듯
    const eDIMENSION_TYPE   m_eDimensionType;

    float m_fGridSize;       //개별 그리드의 사이즈

    float m_fGridLBFPos;     //개별 그리드의 좌하단 위치

    UINT  m_uiGridLevel;

    UINT  m_uiGridSideNum;      //모든 그리드의 가로세로 갯수. 무조건 짝수 단위로 증가(2, 4, 6, ...)
    UINT  m_uiGridSideSquare;   //모든 그리드의 평면의 갯수(z축 계산용)
    UINT  m_uiGridTotalNum;     //모든 그리드의 갯수


    //동적 사이즈 조정 활성화
    bool  m_bDynamicSizeOn;

    //다운사이징을 위한 변수
    UINT  m_uiMaxGridIndex;
    bool  m_bTimeRewind;    //시간을 초기화해야하는지 여부 기록
    float m_fLevelDownTimeRemain;

    //업사이징/다운사이징을 위한 변수
    int  m_uiGridLevelChangeReserve;

    vector<CSpatialGrid>   m_vecGrid;

public:
    bool GetLayerInteract(UINT _iLayer1, UINT _iLayer2);


    //unordered_map<UINT64, bool>::iterator FindCollData(UINT32 _uID1, UINT32 _uID2);


    void SetGridSize(float _fGridSize) { m_fGridSize = _fGridSize; }

    //CCollider에서 호출. 자신의 좌하단과 우상단 위치를 전달하면 해당 값을 통해 어디어디 그리드에 속해있는지를 계산한다.
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
