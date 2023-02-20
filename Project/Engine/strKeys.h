#pragma once

namespace RESOURCE
{
	namespace MESH
	{
		inline constexpr const char* POINT = "PointMesh";
		inline constexpr const char* RECT = "RectMesh";
		inline constexpr const char* RECT_DEBUG = "RectMesh_Debug";
		inline constexpr const char* CIRCLE = "CircleMesh";
		inline constexpr const char* CIRCLE_DEBUG = "CircleMesh_Debug";

	}


	namespace MATERIAL
	{
		inline constexpr const char* DEBUG = "DebugMtrl";
		inline constexpr const char* TEST = "TestMtrl";
		inline constexpr const char* STD2D = "std2DMtrl";
		inline constexpr const char* STD2D_LIGHT = "std2DLightMtrl";
		inline constexpr const char* TILEMAP = "TilemapMtrl";
		inline constexpr const char* PARTICLE_RENDER = "ParticleRenderMtrl";

	}

	namespace SHADER
	{
		inline constexpr const char* DEBUG = "DebugShader";
		inline constexpr const char* TEST = "TestShader";
		inline constexpr const char* STD2D = "std2DShader";
		inline constexpr const char* STD2D_LIGHT = "std2DLightShader";
		inline constexpr const char* TILEMAP = "TilemapShader";
		inline constexpr const char* PARTICLE_RENDER = "ParticleRenderShader";

		namespace COMPUTE
		{
			inline constexpr const char* SET_COLOR = "CSSetColor";
			inline constexpr const char* PARTICLE_UPDATE_BASIC = "CSParticleUpdateBasic";
			inline constexpr const char* PARTICLE_UPDATE_RAINDROP = "CSParticleUpdateRainDrop";
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
		inline constexpr const char* NOISE_TEXTURE_0 = "NoiseTexture";
	}
}