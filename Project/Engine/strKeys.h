#pragma once

namespace RESOURCE
{
	namespace MESH
	{
		inline constexpr const char* POINT = "PointMesh";
		inline constexpr const char* RECT = "RectMesh";
		inline constexpr const char* CIRCLE = "CircleMesh";

		inline constexpr const char* DEBUG_RECT = "RectMesh_Debug";
		inline constexpr const char* DEBUG_CIRCLE = "CircleMesh_Debug";

	}


	namespace MATERIAL
	{
		inline constexpr const char* DEBUG_RECT = "DebugMtrlRect";
		inline constexpr const char* DEBUG_CIRCLE = "DebugMtrlCircle";

		inline constexpr const char* TEST = "TestMtrl";
		inline constexpr const char* STD2D = "std2DMtrl";
		inline constexpr const char* STD2D_LIGHT = "std2DLightMtrl";
		
		inline constexpr const char* TILEMAP_COMPLETE = "TilemapCompleteMtrl";
		inline constexpr const char* TILEMAP_ATLAS = "TilemapAtlasMtrl";

		inline constexpr const char* PARTICLE_RENDER = "ParticleRenderMtrl";

		inline constexpr const char* CORSAIR = "CorsairMtrl";
		inline constexpr const char* MARINE = "MarineMtrl";
	}

	namespace SHADER
	{
		inline constexpr const char* DEBUG = "DebugShader";
		inline constexpr const char* TEST = "TestShader";
		inline constexpr const char* STD2D = "std2DShader";
		inline constexpr const char* STD2D_LIGHT = "std2DLightShader";
		
		inline constexpr const char* TILEMAP_COMPLETE = "TilemapComplete";
		inline constexpr const char* TILEMAP_ATLAS = "TilemapShader";


		inline constexpr const char* PARTICLE_RENDER = "ParticleRenderShader";

		namespace COMPUTE
		{
			inline constexpr const char* INIT_SETTING = "CSInitSetting";
			inline constexpr const char* SET_COLOR = "CSSetColor";
			inline constexpr const char* PARTICLE_UPDATE_BASIC = "CSParticleUpdateBasic";
			inline constexpr const char* PARTICLE_UPDATE_RAINDROP = "CSParticleUpdateRainDrop";
			inline constexpr const char* SC_MAP_LOADER = "CS_SCMapLoader";
		}
	}

	
	namespace TEXTURE
	{
		inline constexpr const char* FIGHTER = "Fighter";
		inline constexpr const char* HOS = "HOS";
		inline constexpr const char* BEHEADED_ATLAS = "beheaded";
		inline constexpr const char* BEHEADED_ATLAS_NORMAL = "beheaded_n";
		inline constexpr const char* TILE_ATLAS = "TileAtlas";
		inline constexpr const char* LINK_ATLAS = "LinkAtlas";
		inline constexpr const char* REAVER_ATLAS = "ReaverAtlas";
		inline constexpr const char* NOISE_TEXTURE_0 = "NoiseTexture";
		inline constexpr const char* CORSAIR_ATLAS = "CorsairAtlas";
		inline constexpr const char* MARINE_ATLAS = "MarineAtlas";
	}

	namespace ANIM2D
	{
		inline constexpr const char* REAVERMOVE = "ReaverMove";
		inline constexpr const char* CORSAIRMOVE = "CorsairMove";

		inline constexpr const char* MARINE_IDLE = "MarineIdle";
		inline constexpr const char* MARINE_MOVE = "MarineMove";
		inline constexpr const char* MARINE_ATTACK = "MarineAttack";
		inline constexpr const char* MARINE_ATTACKSTART = "MarineAttackStart";
		inline constexpr const char* MARINE_DEATH = "MarineDeath";
		
	}
}