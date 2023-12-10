#pragma once
#include <EngineBase/Engine/iComponent.h>

#include "define_SCMap.h"

enum class eTilemap_DebugMode
{
    NONE,
    MEGATILE,
    MINITILE
};



namespace ehw
{
    class StructBuffer;
    class CS_TilemapLoader;
    class Texture;
    class Com_Renderer_TilemapSC :
        public iComponent
    {
    public:
        Com_Renderer_TilemapSC();
        virtual ~Com_Renderer_TilemapSC();

        virtual void Init() override;
        virtual void InternalUpdate() override {}

    private:
        static int m_iRefCount_TilesetData;
        static std::unique_ptr<StructBuffer[]> m_arrSBuffer_TilesetData[(int)SC_Map::eTILESET_INFO::END];

        std::shared_ptr<CS_TilemapLoader> m_TilemapLoader;

        uint m_uNumMegatileX;
        uint m_uNumMegatileY;

        SC_Map::eTILESET_INFO m_eTileSet;

        std::string m_strMapName;


        std::vector<tMegaTile> m_vecMegaTile;
        std::vector<tMiniTile> m_vecMiniTile;
        std::vector<SC_Map::tUnitData> m_vecUnitData;
        std::vector<float2> m_vecStartLocation;

        bool    m_bMapLoaded;
        bool    m_bUnitLoaded;


        //Buffers
        std::shared_ptr<Texture> m_pMapTex;
        std::shared_ptr<StructBuffer> m_pSBuffer_MXTM;
        std::shared_ptr<StructBuffer> m_pSBufferRW_Megatile;
        std::shared_ptr<StructBuffer> m_pSBufferRW_Minitile;
    };
}


