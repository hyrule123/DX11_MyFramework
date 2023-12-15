
#include "CS_TilemapLoaderSC.h"

#if defined (_WIN64) && _DEBUG



#elif defined (_WIN64) && !(_DEBUG)

#endif

namespace ehw
{
	CS_TilemapLoaderSC::CS_TilemapLoaderSC()
		: iComputeShader(typeid(iComputeShader), uint3(32u, 32u, 1u))
	{
	}
	CS_TilemapLoaderSC::~CS_TilemapLoaderSC()
	{
	}
	eResult CS_TilemapLoaderSC::Load(const std::filesystem::path& _filePath)
	{
		return CreateByCSO("CS_SCTilemapLoader.cso");
	}
	eResult CS_TilemapLoaderSC::LoadMap(const std::filesystem::path& _filePath)
	{
		return eResult();
	}

	bool CS_TilemapLoaderSC::BindData()
	{
		return false;
	}

	void CS_TilemapLoaderSC::UnBindData()
	{
	}

	void CS_TilemapLoaderSC::CreateTilesetData()
	{
	}
}
