#pragma once

#include <Engine/CFStateMgr.h>

enum class eSCUNIT_MOVE_TYPE
{
    Ground,
    Hover,
    Air
};

class CSCEntity :
    public CFStateMgr
{
public:
    CSCEntity(std::type_index _typeIdx, UINT _eNumState);
    virtual ~CSCEntity();

private:
    eSCUNIT_MOVE_TYPE m_eSCUnitType;

public:
    //virtual bool LoadJson(Json::Value* _pJVal) override;
};

