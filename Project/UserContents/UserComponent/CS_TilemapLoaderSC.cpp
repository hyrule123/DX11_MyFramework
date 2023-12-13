
#include "CS_TilemapLoaderSC.h"


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
	bool CS_TilemapLoaderSC::BindData()
	{
		return false;
	}
	void CS_TilemapLoaderSC::UnBindData()
	{
	}
}
