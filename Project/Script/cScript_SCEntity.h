#pragma once

#include <Engine/IScript.h>

#include <Engine/cKeyMgr.h>

#include "define_SC.h"

class cScript_SCEntity :
    public IScript
{
public:
    cScript_SCEntity();

    cScript_SCEntity(const cScript_SCEntity& _other) = default;
    CLONE(cScript_SCEntity);

    virtual ~cScript_SCEntity();

public:
    virtual bool SaveJson(Json::Value* _pJVal) override;
    virtual bool LoadJson(Json::Value* _pJVal) override;

public:
    virtual void Init() override;

    virtual void OnCollision(ICollider* _pCol, const Vec3& _v3HitPoint) override;


public:
    DECLARE_GETTER_SETTER(SC::eUNIT_ID, m_eUnitID, UnitID);

    //종족 정보
    DECLARE_GETTER_SETTER(SC::eRACE, m_eRace, Race);
    DECLARE_GETTER_SETTER(UINT, m_uPopulation, Population);

    //이동 타입(지상, 호버링, 공중)
    DECLARE_GETTER_SETTER(SC::eUNIT_MOVE_TYPE, m_eMoveType, MoveType);
    DECLARE_GETTER_SETTER(SC::eUNIT_SIZE_TYPE, m_eUnitSize, UnitSizeType);

    //시야
    DECLARE_GETTER_SETTER(UINT, m_uSightRange, SightRange);

    //생산에 필요한 자원 정보
    DECLARE_GETTER_SETTER(UINT, m_uCostMineral, CostMineral);
    DECLARE_GETTER_SETTER(UINT, m_uCostGas, CostGas);
    DECLARE_GETTER_SETTER(float, m_fBaseBuildTime, BuildTime);

private: 
    string m_strProdBuildingName;
public:
    //생산 가능한 건물 이름
    GETTER_SETTER(const string&, m_strProdBuildingName, strProdBuildingName);

    //생산 단축키
    DECLARE_GETTER_SETTER(eKEY, m_eKeyShortcut, ShortcutKey);

    //생산 조건 테크
    DECLARE_GETTER_SETTER(UINT, m_flagTech, flagTech);


public:


public:
    void Attack(cGameObject* _pTarget);
};
