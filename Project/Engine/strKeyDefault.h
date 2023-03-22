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



namespace RES_DEFAULT
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
		constexpr const std::string_view DEBUG_RECT = "DebugRect.mtrl";
		constexpr const std::string_view DEBUG_CIRCLE = "DebugCircle.mtrl";

		constexpr const std::string_view TEST = "Test.mtrl";
		constexpr const std::string_view STD2D = "std2D.mtrl";
		constexpr const std::string_view STD2D_LIGHT = "std2DLight.mtrl";
		
		constexpr const std::string_view TILEMAP_COMPLETE = "TilemapComplete.mtrl";
		constexpr const std::string_view TILEMAP_ATLAS = "TilemapAtlas.mtrl";

		constexpr const std::string_view PARTICLE_RENDER = "ParticleRender.mtrl";
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
			constexpr const std::string_view SCMAPLOADER = "S_1_C_SCMapLoader.json";
			constexpr const std::string_view INITALIZE = "S_C_Initalize.json";
			constexpr const std::string_view PARTICLEBASIC = "S_C_ParticleBasic.json";
			constexpr const std::string_view PARTICLERAINDROP = "S_C_ParticleRainDrop.json";
			constexpr const std::string_view SETCOLOR = "S_C_SetColor.json";
		}
	}
	
	namespace TEXTURE
	{
		constexpr const std::string_view DEFAULT = "Default.png";
		constexpr const std::string_view NOISE_1 = "Noise/noise_01.png";
	}
}


namespace RES_INFO
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
		constexpr const std::string_view ExtensionArr[(int)eTEX_TYPE::END] = { ".png", ".bmp" };
	}

	namespace SHADER
	{
		constexpr std::string_view DirNameRoot = "Shader";
		constexpr std::string_view Extension_ShaderSetting = ".json";
		constexpr std::string_view Extension_ShaderCode = ".cso";

		namespace GRAPHICS
		{
			constexpr const std::string_view DirName = "Graphics";
			constexpr std::string_view PrefixArr[(int)eSHADER_TYPE::END] =
			{ "_V_" , "_H_" , "_D_" ,"_G_" ,"_P_" };

			//각 쉐이더마다 들어가야할 설정값

			namespace Setting
			{
				//쉐이더 파일의 '이름'만 추출
				//ex) S_1_V_Debug : Debug
				//이름 : CEntity의 Name을 사용
				constexpr std::string_view ePipelineFlag = "eSHADER_PIPELINE_STAGE";
				constexpr std::string_view eTopology = "D3D_PRIMITIVE_TOPOLOGY";
				constexpr std::string_view eRSState = "eRASTERIZER_TYPE";
				constexpr std::string_view eDSState = "eDEPTHSTENCIL_TYPE";
				constexpr std::string_view eBState = "eBLENDSTATE_TYPE";
				constexpr std::string_view eShaderDomain = "eSHADER_DOMAIN";
			}
		}

		namespace COMPUTE
		{
			constexpr const std::string_view DirName = "Compute";
			constexpr std::string_view Prefix = "_C_";

			namespace Setting
			{
				//쉐이더와 json 파일이 공유하는 파일 이름. 확장자 제외
				//ex) S_C_SCMapLoader.json : S_C_SCMapLoader
				constexpr const std::string_view ShaderFileName = "ShaderFileName";
				constexpr const std::string_view iNumThreadArr = "NumThreadArr";
			}
		}

		
		
	
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
		constexpr const std::string_view arrExtension[(int)eSCMAP_TYPE::END] = { ".scm", ".scx" };
	}

	namespace MATERIAL
	{
		constexpr const std::string_view DirName = "Material";
		constexpr const std::string_view Ext = ".json";
	}
}

