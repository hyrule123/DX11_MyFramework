#pragma once

#include <Engine/CFStateMgr.h>
#include <Engine/CKeyMgr.h>

#include "define_SCUnit.h"

class SCUnitCom_Base;
class CSC_Entity :
    public CFStateMgr
{
public:
    CSC_Entity(std::type_index _typeIdx, UINT _eNumState);

    CSC_Entity(const CSC_Entity& _other) = default;
    CLONE(CSC_Entity);

    virtual ~CSC_Entity();

public:
    virtual bool SaveJson(Json::Value* _pJVal) override;
    virtual bool LoadJson(Json::Value* _pJVal) override;

public:
    virtual void initFSM() override;

public:
    virtual void OnCollision(CCollider* _pCol, const Vec3& _v3HitPoint) override;

private:
    //FSM: 부모 클래스에 정의되어 있음

    //유닛 컴포넌트 모음
    vector<SCUnitCom_Base*> m_vecUnitComponent;

    DECLARE_GETSET(eSCUNIT_UNIT_RACE, m_eUnitType, UnitType);
    DECLARE_GETSET(UINT, m_uPopulation, Population);

    DECLARE_GETSET(eSCUNIT_MOVE_TYPE, m_eMoveType, MoveType);
    DECLARE_GETSET(eSCUNIT_UNIT_SIZE_TYPE, m_eUnitSizeType, UnitSizeType);

    DECLARE_GETSET(UINT, m_uPopulation, Population);
    DECLARE_GETSET(UINT, m_uSightRange, SightRange);
    DECLARE_GETSET(UINT, m_uCostMineral, CostMineral);
    DECLARE_GETSET(UINT, m_uCostGas, CostGas);
    DECLARE_GETSET(float, m_fBaseBuildTime, BuildTime);

private: 
    string m_strProdBuildingName;
public:
    GETSET(const string&, m_strProdBuildingName, strProdBuildingName);

    DECLARE_GETSET(eKEY, m_eKeyShortcut, ProdSlot);
    DECLARE_GETSET(UINT, m_flagTech, flagTech);

    //종족 + 인구
    eSCUNIT_UNIT_RACE m_eUnitType;
    UINT m_uPopulation;

    //이동 타입
    eSCUNIT_MOVE_TYPE m_eMoveType;

    //유닛 사이즈 정보(소/중/대)
    eSCUNIT_UNIT_SIZE_TYPE m_eUnitSizeType;

    //메가타일(32px)기준 시야 범위
    UINT m_uSightRange;

    //생산 미네랄
    UINT m_uCostMineral;
    UINT m_uCostGas;

    //빌드타임
    float m_fBaseBuildTime;

    //유닛에 따라 있을 수도 있고 없을 수도 있음
    string m_strProdBuildingName;
    //UI의 몇 번 칸에 출력되는지
    UINT m_ProdSlot;
    eKEY m_eKeyShortcut;

    //생산 조건 flag(테크) - 나중에 사용 예정
    UINT m_flagTech;

public:


private:

};

