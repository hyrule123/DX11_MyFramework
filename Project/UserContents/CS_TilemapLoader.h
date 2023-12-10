#pragma once
#include <EngineBase/Engine/iComputeShader.h>

namespace ehw
{
    class CS_TilemapLoader :
        public iComputeShader
    {
    public:
        CS_TilemapLoader();
        virtual ~CS_TilemapLoader();

        virtual eResult Load(const std::filesystem::path& _filePath) override;

    protected:
        virtual bool BindData() override;
        virtual void UnBindData() override;



    };
}


