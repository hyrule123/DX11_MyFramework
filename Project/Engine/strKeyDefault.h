#pragma once


#define MAKE_RELATIVE_PATH(_NameSpace, _strPath) \
namespace _NameSpace \
{ \
constexpr const std::u8string_view U8 = u8#_strPath;\
constexpr const std::string_view A = #_strPath;\
constexpr const std::wstring_view W = L#_strPath;\
}

namespace RELATIVE_PATH
{
	MAKE_RELATIVE_PATH(CONTENT, ./Content);
	MAKE_RELATIVE_PATH(SHADER_GRAPHICS, ./Content/Shader/Graphics);
	MAKE_RELATIVE_PATH(SHADER_COMPUTE, ./Content/Shader/Compute);
	MAKE_RELATIVE_PATH(TEXTURE, ./Content/Texture);
	MAKE_RELATIVE_PATH(SCMAP, ./Content/SCMap);
	MAKE_RELATIVE_PATH(MATERIAL, ./Material);
}

namespace DIRECTORY_NAME
{
	MAKE_RELATIVE_PATH(CONTENT, Content);
	MAKE_RELATIVE_PATH(SHADER, Shader);
	MAKE_RELATIVE_PATH(GRAPHICS, Graphics);
	MAKE_RELATIVE_PATH(COMPUTE, Compute);
	MAKE_RELATIVE_PATH(TEXTURE, Texture);
	MAKE_RELATIVE_PATH(SCMAP, Maps);
	MAKE_RELATIVE_PATH(SAVED_SETTINGS, SavedSetting);
	MAKE_RELATIVE_PATH(MATERIAL, Material);

}



namespace DEFAULT_RES
{



	namespace MESH
	{
		constexpr const std::string_view POINT = "PointMesh";
		constexpr const std::string_view RECT = "RectMesh";
		constexpr const std::string_view CIRCLE = "CircleMesh";

		constexpr const std::string_view DEBUG_RECT = "RectMesh_Debug";
		constexpr const std::string_view DEBUG_CIRCLE = "CircleMesh_Debug";
	}


	namespace MATERIAL
	{
		constexpr const std::string_view DEBUG_RECT = "DebugMtrlRect";
		constexpr const std::string_view DEBUG_CIRCLE = "DebugMtrlCircle";

		constexpr const std::string_view TEST = "TestMtrl";
		constexpr const std::string_view STD2D = "std2DMtrl";
		constexpr const std::string_view STD2D_LIGHT = "std2DLightMtrl";
		
		constexpr const std::string_view TILEMAP_COMPLETE = "TilemapCompleteMtrl";
		constexpr const std::string_view TILEMAP_ATLAS = "TilemapAtlasMtrl";

		constexpr const std::string_view PARTICLE_RENDER = "ParticleRenderMtrl";
	}

	namespace SHADER
	{
		namespace GRAPHICS
		{
			constexpr const std::string_view DEBUG = "Debug.json";
			constexpr const std::string_view PARTICLE = "Particle.json";
			constexpr const std::string_view STD2D = "STD2D.json";
			constexpr const std::string_view STD2DLIGHT = "STD2DLight.json";
			constexpr const std::string_view TEST = "Test.json";
			constexpr const std::string_view TILEMAPATLAS = "TilemapAtlas.json";
			constexpr const std::string_view TILEMAPCOMPLETE = "TilemapComplete.json";
		}

		namespace COMPUTE
		{
			constexpr const std::string_view SCMAPLOADER = "S_1_C_SCMapLoader.cso";
			constexpr const std::string_view INITALIZE = "S_C_Initalize.cso";
			constexpr const std::string_view PARTICLEBASIC = "S_C_ParticleBasic.cso";
			constexpr const std::string_view PARTICLERAINDROP = "S_C_ParticleRainDrop.cso";
			constexpr const std::string_view SETCOLOR = "S_C_SetColor.cso";
		}
	}
	
	namespace TEXTURE
	{
		constexpr const std::string_view DEFAULT = "Default.png";
		constexpr const std::string_view NOISE_1 = "Noise/noise_01.png";
	}
}


namespace RESOURCE_INFO
{
	
	//순회 돌 떄 편하게 돌수있도록 배열 형태로 사용
	namespace TEXTURE
	{
		enum class eTEX_TYPE
		{
			PNG,
			BMP,
			END
		};
		constexpr const std::string_view DirName = "Texture";
		constexpr const std::string_view arrExt[(int)eTEX_TYPE::END] = { ".png", ".bmp" };
	}

	namespace SHADER
	{
		constexpr const std::string_view DirName_Graphics = "Graphics";
		constexpr const std::string_view DirName_Compute = "Compute";
		
		constexpr const std::string_view SHADER_CODE = ".cso";
		constexpr const std::string_view SHADER_INFO = ".json";
	}


	namespace SCMAP
	{
		constexpr const std::string_view DirName = "SCMap";

		enum class eSCMAP_TYPE
		{
			SCM,
			SCX,
			END
		};
		constexpr const std::string_view arrExt[(int)eSCMAP_TYPE::END] = { ".scm", ".scx" };
	}

	constexpr const std::string_view MATERIAL = ".json";
}

