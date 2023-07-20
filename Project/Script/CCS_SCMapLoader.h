#pragma once

#include <Engine/cShaderDataModule.h>

#include <HLSL/S_H_SCMapLoader.hlsli>

#include "define_SCMap.h"

class cStructBuffer;
class CCS_SCMapLoader :
    public cShaderDataModule
{

public:
    CCS_SCMapLoader();
    virtual ~CCS_SCMapLoader();

public:
    virtual tNumDataCS BindDataCS() override;
    virtual void UnBind() override;
    
private:
    
    SC_Map::tMapData m_Map;

public:
    //Map 폴더 안의 맵 이름을 인자로 받아서 맵을 로드함.
    shared_ptr<SC_Map::tMapData> LoadMap(const string_view _strMapName);

private:
    
    

    
};

