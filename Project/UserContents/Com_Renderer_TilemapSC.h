#pragma once

#include <EngineBase/Engine/IComponent.h>

enum class eTilemap_DebugMode
{
    NONE,
    MEGATILE,
    MINITILE
};

namespace ehw
{
    class CS_TilemapLoader;
    class Com_Renderer_TilemapSC :
        public IComponent
    {
    public:
        Com_Renderer_TilemapSC();
        virtual ~Com_Renderer_TilemapSC();

        virtual void Init() override;
        virtual void FixedUpdate() override {}

    private:
        std::shared_ptr<CS_TilemapLoader> m_TilemapLoaderCS;


    };
}


