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
    virtual ~cCom_Renderer_TilemapSC();

    cCom_Renderer_TilemapSC(const cCom_Renderer_TilemapSC& _other);
    CLONE(cCom_Renderer_TilemapSC);

public:
    virtual void Init() override;
    virtual void Tick() override;
    virtual eRENDER_RESULT Render() override;

private:
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

    class cMapDataModule : public cShaderDataModule
    {
    public:
        cMapDataModule() {}
        virtual ~cMapDataModule() {}

        Ptr<cTexture> m_pMapTex;
        SC_Map::tpSBufferTileSet m_arrpSBufferTileSet[(int)SC_Map::eTILESET_INFO::END];
        std::unique_ptr<cStructBuffer> m_pSBuffer_MXTM;
        std::unique_ptr<cStructBuffer> m_pSBufferRW_Megatile;
        std::unique_ptr<cStructBuffer> m_pSBufferRW_Minitile;

        tNumDataCS BindDataCS() override;
        bool BindDataGS() override;
        void UnBind() override;
    };
    cMapDataModule m_MapDataModule;

public:
    bool LoadMap(const string_view _strMapName);
    

private:
    eTILEMAP_DEBUGMODE m_eDebugMode;
public:
    void SetTilemapDebugMode(eTILEMAP_DEBUGMODE _eMode) { m_eDebugMode = _eMode; }


private:
    std::shared_ptr<SC_Map::tMapRawData> ExtractMap(const std::filesystem::path& _MapFilePath);
    bool ReadMapData(std::shared_ptr<SC_Map::tMapRawData> _RawData);

    std::shared_ptr<SC_Map::tMapDataChunk> MultiThread_CopyChunk(const std::string& _dataStr, SC_Map::eSCMAP_DATA_TYPE _eDataType);

    bool UploadMapDataToCS();

    void UnLoad();
    void LoadUnit();
    void UnLoadUnit();

    void StartLocation();
};

