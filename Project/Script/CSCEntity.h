#pragma once

#include <Engine/CFStateMgr.h>
#include <Engine/CKeyMgr.h>

#include "define_SCUnit.h"



class CSCEntity :
    public CFStateMgr
{
public:
    CSCEntity(std::type_index _typeIdx, UINT _eNumState);
    //CSCEntity(const CSCEntity& _other); //기본 복사생성자로 충분함.
    virtual ~CSCEntity();

    CLONE(CSCEntity);

public:
    virtual void initFSM() override;

public:
    virtual void OnCollision(CCollider* _pCol) override;

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
    GETSET(eSCUNIT_UNIT_TYPE, UnitType, m_eUnitType);
    GETSET(eSCUNIT_MOVE_TYPE, MoveType, m_eMoveType);
    GETSET(eSCUNIT_UNIT_SIZE_TYPE, UnitSizeType, m_eUnitSizeType);

    GETSET(UINT, Population, m_uPopulation);
    GETSET(UINT, SightRange, m_uSightRange);
    GETSET(UINT, CostMineral, m_uCostMineral);
    GETSET(UINT, CostGas, m_uCostGas);
    GETSET(float, BuildTime, m_fBaseBuildTime);

    GETSET(const string&, strProdBuildingName, m_strProdBuildingName);
    GETSET(eKEY, ProdSlot, m_eKeyShortcut);
    GETSET(UINT, flagTech, m_flagTech);

private:

};

