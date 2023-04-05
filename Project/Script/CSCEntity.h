#pragma once

#include <Engine/CFStateMgr.h>
#include <Engine/CKeyMgr.h>

#include "define_SCUnit.h"



class CSCEntity :
    public CFStateMgr
{
public:
    CSCEntity(std::type_index _typeIdx, UINT _eNumState);

    CSCEntity(const CSCEntity& _other) = default;
    CLONE(CSCEntity);

    virtual ~CSCEntity();

    

public:
    virtual void initFSM() override;

public:
    virtual void OnCollision(CCollider* _pCol, const Vec3& _v3HitPoint) override;

private:
    eSCUNIT_UNIT_TYPE m_eUnitType;
    UINT m_uPopulation;

    eSCUNIT_MOVE_TYPE m_eMoveType;
    eSCUNIT_UNIT_SIZE_TYPE m_eUnitSizeType;

    //시야. 32를 곱해줘야 함
    UINT m_uSightRange;

    UINT m_uCostMineral;
    UINT m_uCostGas;

    float m_fBaseBuildTime;

    //유닛에 따라 있을 수도 있고 없을 수도 있음
    string m_strProdBuildingName;
    //UI의 몇 번 칸에 출력되는지
    UINT m_ProdSlot;
    eKEY m_eKeyShortcut;

    //생산 조건 flag(테크) - 나중에 사용 예정
    UINT m_flagTech;

public:
    GETSET(eSCUNIT_UNIT_TYPE, m_eUnitType, UnitType);
    GETSET(eSCUNIT_MOVE_TYPE, m_eMoveType, MoveType);
    GETSET(eSCUNIT_UNIT_SIZE_TYPE, m_eUnitSizeType, UnitSizeType);

    GETSET(UINT, m_uPopulation, Population);
    GETSET(UINT, m_uSightRange, SightRange);
    GETSET(UINT, m_uCostMineral, CostMineral);
    GETSET(UINT, m_uCostGas, CostGas);
    GETSET(float, m_fBaseBuildTime, BuildTime);

    GETSET(const string&, m_strProdBuildingName, strProdBuildingName);
    GETSET(eKEY, m_eKeyShortcut, ProdSlot);
    GETSET(UINT, m_flagTech, flagTech);

private:

};

