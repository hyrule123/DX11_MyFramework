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
    class ComputeShader;
    class Com_Renderer_TilemapSC :
        public IComponent
    {
    public:
        Com_Renderer_TilemapSC();
        virtual ~Com_Renderer_TilemapSC();

        virtual void FixedUpdate() override {}

    private:
        
    };
}


