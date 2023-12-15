#pragma once
#include <Engine/Resource/Shader/iComputeShader.h>

namespace ehw
{
    class CS_TilemapLoaderSC :
        public iComputeShader
    {
    public:
        CS_TilemapLoaderSC();
        virtual ~CS_TilemapLoaderSC();

        virtual eResult Load(const std::filesystem::path& _filePath) override;

        eResult LoadMap(const std::filesystem::path& _filePath);

    protected:
        virtual bool BindData() override;
        virtual void UnBindData() override;

    private:
        void CreateTilesetData();

    };
}


