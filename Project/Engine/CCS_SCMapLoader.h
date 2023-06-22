#pragma once

#include "CComputeShader.h"

#include "S_H_SCMapLoader.hlsli"

#include "SCMapDefine.h"


class CStructBuffer;
class CCS_SCMapLoader :
    public CComputeShader
{
public:
    CCS_SCMapLoader();
    virtual ~CCS_SCMapLoader();

public:
    virtual bool BindDataCS() override;
    virtual void UnBindCS() override;
    
private:
    SC_Map::tpSBufferTileSet m_arrpSBufferTileSet[(int)SC_Map::eTILESET_INFO::END];

    //============맵 정보 작업 영역=============
    shared_ptr<SC_Map::tMapData> m_pMapWorkSpace;

public:
    //Map 폴더 안의 맵 이름을 인자로 받아서 맵을 로드함.
    shared_ptr<SC_Map::tMapData> LoadMap(const string_view _strMapName);

private:
    bool ReadMapData(char* Data, DWORD Size);
    bool UploadMapDataToCS();

    std::shared_ptr<SC_Map::tMapDataChunk> MultiThread_CopyChunk(const std::string& _dataStr, SC_Map::eSCMAP_DATA_TYPE _eDataType);
};

