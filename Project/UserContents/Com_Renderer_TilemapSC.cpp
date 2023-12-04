#include "PCH_UserContents.h"
#include "Com_Renderer_TilemapSC.h"

#include "CS_TilemapLoader.h"

#include <EngineBase/Engine/ResourceMgr.h>



namespace ehw
{
	Com_Renderer_TilemapSC::Com_Renderer_TilemapSC()
		: IComponent(eComponentCategory::Renderer)
	{
	}
	Com_Renderer_TilemapSC::~Com_Renderer_TilemapSC()
	{
	}
	void Com_Renderer_TilemapSC::Init()
	{
		m_TilemapLoader = ResourceMgr::Load<CS_TilemapLoader>("CS_TilemapLoader");
	}
}
