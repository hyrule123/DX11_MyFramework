#pragma once

namespace DEFAULT_RES
{
	namespace MESH
	{
		constexpr const char* POINT = "PointMesh";
		constexpr const char* RECT = "RectMesh";
		constexpr const char* CIRCLE = "CircleMesh";

		constexpr const char* DEBUG_RECT = "RectMesh_Debug";
		constexpr const char* DEBUG_CIRCLE = "CircleMesh_Debug";
	}


	namespace MATERIAL
	{
		constexpr const char* DEBUG_RECT = "DebugMtrlRect";
		constexpr const char* DEBUG_CIRCLE = "DebugMtrlCircle";

		constexpr const char* TEST = "TestMtrl";
		constexpr const char* STD2D = "std2DMtrl";
		constexpr const char* STD2D_LIGHT = "std2DLightMtrl";
		
		constexpr const char* TILEMAP_COMPLETE = "TilemapCompleteMtrl";
		constexpr const char* TILEMAP_ATLAS = "TilemapAtlasMtrl";

		constexpr const char* PARTICLE_RENDER = "ParticleRenderMtrl";

		constexpr const char* CORSAIR = "CorsairMtrl";
		constexpr const char* MARINE = "MarineMtrl";
	}

	namespace SHADER
	{
		constexpr const char* DEBUG = "DebugShader";
		constexpr const char* TEST = "TestShader";
		constexpr const char* STD2D = "std2DShader";
		constexpr const char* STD2D_LIGHT = "std2DLightShader";
		
		constexpr const char* TILEMAP_COMPLETE = "TilemapComplete";
		constexpr const char* TILEMAP_ATLAS = "TilemapShader";


		constexpr const char* PARTICLE_RENDER = "ParticleRenderShader";


		namespace COMPUTE
		{
			constexpr const char* INIT_SETTING = "CSInitSetting";
			constexpr const char* SET_COLOR = "CSSetColor";
			constexpr const char* PARTICLE_UPDATE_BASIC = "CSParticleUpdateBasic";
			constexpr const char* PARTICLE_UPDATE_RAINDROP = "CSParticleUpdateRainDrop";
			constexpr const char* SC_MAP_LOADER = "CS_SCMapLoader";
		}
	}

	
	namespace TEXTURE
	{
		constexpr const char* FIGHTER = "Fighter";
		constexpr const char* HOS = "HOS";
		constexpr const char* BEHEADED_ATLAS = "beheaded";
		constexpr const char* BEHEADED_ATLAS_NORMAL = "beheaded_n";
		constexpr const char* TILE_ATLAS = "TileAtlas";
		constexpr const char* LINK_ATLAS = "LinkAtlas";
		constexpr const char* REAVER_ATLAS = "ReaverAtlas";
		constexpr const char* NOISE_TEXTURE_0 = "NoiseTexture";
		constexpr const char* CORSAIR_ATLAS = "CorsairAtlas";
		constexpr const char* MARINE_ATLAS = "MarineAtlas";
	}

	namespace ANIM2D
	{
		constexpr const char* REAVERMOVE = "ReaverMove";
		constexpr const char* CORSAIRMOVE = "CorsairMove";

		constexpr const char* MARINE_IDLE = "MarineIdle";
		constexpr const char* MARINE_MOVE = "MarineMove";
		constexpr const char* MARINE_ATTACK = "MarineAttack";
		constexpr const char* MARINE_ATTACKSTART = "MarineAttackStart";
		constexpr const char* MARINE_DEATH = "MarineDeath";
	}
}