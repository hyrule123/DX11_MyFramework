#pragma once

enum class eRES_TYPE
{
	UNKNOWN = -1,

	MESH,			// 형태
	MESHDATA,
	MATERIAL,

	TEXTURE,		// 이미지
	ANIM2D_ATLAS,
	SCMAP,

	SOUND,

	PREFAB,

	GRAPHICS_SHADER,
	COMPUTE_SHADER,

	END
};


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
		constexpr const std::string_view DEBUG_RECT = "DebugRect.json";
		constexpr const std::string_view DEBUG_CIRCLE = "DebugCircle.json";

		constexpr const std::string_view TEST = "Test.json";
		constexpr const std::string_view STD2D = "std2D.json";
		constexpr const std::string_view STD2D_LIGHT = "std2DLight.json";
		
		constexpr const std::string_view TILEMAP_COMPLETE = "TilemapComplete.json";
		constexpr const std::string_view TILEMAP_ATLAS = "TilemapAtlas.json";

		constexpr const std::string_view PARTICLE_RENDER = "ParticleRender.json";
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
	namespace MESHDATA
	{
		enum class eEXT_TYPE
		{
			FBX,
			END
		};

		constexpr const std::string_view arrExt[(int)eEXT_TYPE::END] = { ".fbx" };

		constexpr const std::string_view DirName = DIRECTORY_NAME::RES_ARR[(int)eRES_TYPE::MESH];
	}

	namespace MATERIAL
	{
		constexpr const std::string_view Ext = ".json";

		constexpr const std::string_view DirName = DIRECTORY_NAME::MATERIAL;
	}
	
	//순회 돌 떄 편하게 돌수있도록 배열 형태로 사용
	namespace TEXTURE
	{
		enum class eEXT_TYPE
		{
			PNG,
			BMP,
			END
		};
		
		constexpr const std::string_view arrExt[(int)eEXT_TYPE::END] = { ".png", ".bmp" };
		constexpr const std::string_view DirName = DIRECTORY_NAME::TEXTURE;
	}

	namespace ANIM2D
	{
		constexpr const std::string_view Ext = ".json";
	}

	namespace SOUND
	{
		enum class eEXT_TYPE
		{
			MP3,
			OGG,
			END
		};

		constexpr const std::string_view arrExt[(int)eEXT_TYPE::END] = { ".mp3", ".ogg" };

		constexpr const std::string_view DirName = DIRECTORY_NAME::SOUND;
	}

	namespace PREFAB
	{
		constexpr const std::string_view Ext = ".json";

		constexpr const std::string_view DirName = DIRECTORY_NAME::PREFAB;
	}


	namespace SHADER
	{
		constexpr std::string_view DirNameRoot = "Shader";
		constexpr std::string_view Ext_ShaderSetting = ".json";
		constexpr std::string_view Ext_ShaderCode = ".cso";

		namespace GRAPHICS
		{
			constexpr const std::string_view DirName = DIRECTORY_NAME::SHADER_GRAPHICS;
			constexpr std::string_view arrPrefix[(int)eSHADER_TYPE::END] =
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
		constexpr const std::string_view arrExt[(int)eSCMAP_TYPE::END] = { ".scm", ".scx" };
	}
}

