#include "PCH_UserContents.h"
#include "CS_TilemapLoader.h"


namespace ehw
{
	CS_TilemapLoader::CS_TilemapLoader()
		: ComputeShader(typeid(ComputeShader), uint3(32u, 32u, 1u))
	{
	}
	CS_TilemapLoader::~CS_TilemapLoader()
	{
	}
	eResult CS_TilemapLoader::Load(const std::filesystem::path& _filePath)
	{
		return CreateByCSO("CS_SCTilemapLoader.cso");
	}
	bool CS_TilemapLoader::BindData()
	{
		return false;
	}
	void CS_TilemapLoader::UnBindData()
	{
	}
}
