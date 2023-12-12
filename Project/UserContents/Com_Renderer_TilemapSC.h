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
    class CS_TilemapLoaderSC;
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
        void CreateTilesetData();

    private:
        //이 클래스 전체가 공유해서 사용하는 데이터(이 클래스가 하나도 없을경우 메모리 해제)
        static std::array<std::shared_ptr<StructBuffer[]>, (int)scMap::eTilesetInfo::END> 
            s_arrSBuffer_TilesetData;

        std::array<std::shared_ptr<StructBuffer[]>, (int)scMap::eTilesetInfo::END> m_arrSBuffer_TilesetData;
      
        std::shared_ptr<CS_TilemapLoaderSC> m_TilemapLoader;

        uint m_uNumMegatileX;
        uint m_uNumMegatileY;

        scMap::eTilesetInfo m_eTileSet;

        std::string m_strMapName;


        std::vector<tMegaTile> m_vecMegaTile;
        std::vector<tMiniTile> m_vecMiniTile;
        std::vector<scMap::tUnitData> m_vecUnitData;
        std::vector<float2> m_vecStartLocation;

        bool    m_bMapLoaded;
        bool    m_bUnitLoaded;


        //Buffers
        std::shared_ptr<Texture> m_pMapTex;
        std::shared_ptr<StructBuffer> m_SBuffer_MXTM;
        std::shared_ptr<StructBuffer> m_SBufferRW_Megatile;
        std::shared_ptr<StructBuffer> m_SBufferRW_Minitile;
    };
}


