#pragma once

#include <Engine/cCom_Renderer_TilemapComplete.h>

#include "define_SCMap.h"

enum class eTILEMAP_DEBUGMODE
{
    NONE,
    MEGATILE,
    MINITILE
};

class cComputeShader;
class cCom_Renderer_TilemapSC :
    public cCom_Renderer_TilemapComplete
{
public:
    cCom_Renderer_TilemapSC();

    cCom_Renderer_TilemapSC(const cCom_Renderer_TilemapSC& _other);
    CLONE(cCom_Renderer_TilemapSC);

    virtual ~cCom_Renderer_TilemapSC();

public:
    virtual void Init() override;
    virtual void Tick() override;
    virtual eRENDER_RESULT Render() override;

private:
    static int m_iRefCount_TilesetData;
    static std::unique_ptr<cStructBuffer[]> m_arrSBuffer_TilesetData[(int)SC_Map::eTILESET_INFO::END];

    Ptr<cComputeShader> m_CS_MapLoader;

    //타일 크기는 부모 클래스에 있음.
    //UINT32 m_uNumMegatileX;
    //UINT32 m_uNumMegatileY;

    SC_Map::eTILESET_INFO m_eTileSet;

    string m_strMapName;
    

    vector<tMegaTile> m_vecMegaTile;
    vector<tMiniTile> m_vecMiniTile;
    vector<SC_Map::tUnitData> m_vecUnitData;
    vector<Vec2> m_vecStartLocation;

    bool    m_bMapLoaded;
    bool    m_bUnitLoaded;


    //Buffers
    Ptr<cTexture> m_pMapTex;
    std::shared_ptr<cStructBuffer> m_pSBuffer_MXTM;
    std::shared_ptr<cStructBuffer> m_pSBufferRW_Megatile;
    std::shared_ptr<cStructBuffer> m_pSBufferRW_Minitile;

public:
    bool LoadMap(const string_view _strMapName);
    

private:
    eTILEMAP_DEBUGMODE m_eDebugMode;
public:
    void SetTilemapDebugMode(eTILEMAP_DEBUGMODE _eMode) { m_eDebugMode = _eMode; }


private:
    bool CreateTilesetData();

    std::shared_ptr<SC_Map::tMapRawData> ExtractMap(const std::filesystem::path& _MapFilePath);
    bool ReadMapData(std::shared_ptr<SC_Map::tMapRawData> _RawData);

    std::shared_ptr<SC_Map::tMapDataChunk> MultiThread_CopyChunk(const std::string& _dataStr, SC_Map::eSCMAP_DATA_TYPE _eDataType);

    bool CreateMapbuffers();

    void PrepareMap();
   

    void UnLoad();
    void LoadUnit();
    void UnLoadUnit();
    void StartLocation();
};

