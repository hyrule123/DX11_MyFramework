#pragma once

#include <Engine/CFSM_Mgr.h>
#include <Engine/CKeyMgr.h>

#include "define_SCUnit.h"

#include "SCUnitCom_Base.h"

class CSC_Entity :
    public CFSM_Mgr
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
    SCUnitCom_Base* m_vecUnitComponent[(int)eSCUnit_ComType::END];
    template <typename T>
    T* AddComponent();
public:
    //종족 정보
    DECLARE_GETSET(eSCUNIT_UNIT_RACE, m_eUnitType, UnitType);
    DECLARE_GETSET(UINT, m_uPopulation, Population);

    //이동 타입(지상, 호버링, 공중)
    DECLARE_GETSET(eSCUNIT_MOVE_TYPE, m_eMoveType, MoveType);
    DECLARE_GETSET(eSCUNIT_UNIT_SIZE_TYPE, m_eUnitSizeType, UnitSizeType);

    //시야
    DECLARE_GETSET(UINT, m_uSightRange, SightRange);

    //생산에 필요한 자원 정보
    DECLARE_GETSET(UINT, m_uCostMineral, CostMineral);
    DECLARE_GETSET(UINT, m_uCostGas, CostGas);
    DECLARE_GETSET(float, m_fBaseBuildTime, BuildTime);

private: 
    string m_strProdBuildingName;
public:
    //생산 가능한 건물 이름
    GETSET(const string&, m_strProdBuildingName, strProdBuildingName);

    //생산 단축키
    DECLARE_GETSET(eKEY, m_eKeyShortcut, ProdSlot);

    //생산 조건 테크
    DECLARE_GETSET(UINT, m_flagTech, flagTech);


public:


private:
};

template<typename T>
inline T* CSC_Entity::AddComponent()
{
    static_assert(std::is_base_of<SCUnitCom_Base, T>::value);

    //T* newT = new T()

    //m_vecUnitComponent.pu

    return nullptr;
}
