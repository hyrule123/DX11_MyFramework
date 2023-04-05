#pragma once

#include "define.h"

//이 열거체 중간에 값을 추가하거나 수정할 경우 반드시 밑의 경로 정보도 수정할 것
//이 열거체를 배열의 인덱스로 사용하고 있음.
enum class eRES_TYPE
{
	UNKNOWN = -1,

	MESH,			// 형태
	MESHDATA,
	MATERIAL,

	TEXTURE,		// 이미지
	ANIM2D_ATLAS,

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
		constexpr std::string_view POINT = "PointMesh";
		constexpr std::string_view RECT = "RectMesh";
		constexpr std::string_view CIRCLE = "CircleMesh";

		constexpr std::string_view DEBUG_RECT = "RectMesh_Debug";
		constexpr std::string_view DEBUG_CIRCLE = "CircleMesh_Debug";
	}


	namespace MATERIAL
	{
		constexpr std::string_view DEBUG_RECT = "DebugRect.json";
		constexpr std::string_view DEBUG_CIRCLE = "DebugCircle.json";

		constexpr std::string_view TEST = "Test.json";
		constexpr std::string_view STD2D = "std2D.json";
		constexpr std::string_view STD2D_LIGHT = "std2DLight.json";
		
		constexpr std::string_view TILEMAP_COMPLETE = "TilemapComplete.json";
		constexpr std::string_view TILEMAP_ATLAS = "TilemapAtlas.json";

		constexpr std::string_view PARTICLE_RENDER = "ParticleRender.json";
	}

	namespace SHADER
	{
		namespace GRAPHICS
		{
			constexpr std::string_view DEBUG = "Debug.json";
			constexpr std::string_view PARTICLE = "Particle.json";
			constexpr std::string_view STD2D = "STD2D.json";
			constexpr std::string_view STD2DLIGHT = "STD2DLight.json";
			constexpr std::string_view TEST = "Test.json";
			constexpr std::string_view TILEMAPATLAS = "TilemapAtlas.json";
			constexpr std::string_view TILEMAPCOMPLETE = "TilemapComplete.json";
		}

		namespace COMPUTE
		{
			constexpr std::string_view SCMAPLOADER = "S_1_C_SCMapLoader.json";
			constexpr std::string_view INITALIZE = "S_C_Initalize.json";
			constexpr std::string_view PARTICLEBASIC = "S_C_ParticleBasic.json";
			constexpr std::string_view PARTICLERAINDROP = "S_C_ParticleRainDrop.json";
			constexpr std::string_view SETCOLOR = "S_C_SetColor.json";
		}
	}
	
	namespace TEXTURE
	{
		constexpr std::string_view DEFAULT = "Default.png";
		constexpr std::string_view NOISE_1 = "Noise/noise_01.png";
	}
}

namespace DIRECTORY_NAME
{
	//인덱스는 eRES_TYPE를 사용
	//ResMgr에서 관리되는 리소스의 키 및 폴더 이름으로 사용
	constexpr std::string_view RES_ARR[(int)eRES_TYPE::END] =
	{
"MeshData",
"MeshData",
"Material",
"Texture",
"Anim2D",
"Sound",
"Prefab",
"Shader/Graphics",
"Shader/Compute",
	};

#ifdef _DEBUG
	constexpr std::string_view CONFIGURATION = "_Debug";
#else
	constexpr std::string_view CONFIGURATION = "_Release";
#endif

	constexpr std::string_view RELATIVE_BEGIN = ".";
	constexpr std::string_view CONTENT = "Content";
	constexpr std::string_view MESHDATA = RES_ARR[(int)eRES_TYPE::MESHDATA];
	constexpr std::string_view MATERIAL = RES_ARR[(int)eRES_TYPE::MATERIAL];
	constexpr std::string_view TEXTURE = RES_ARR[(int)eRES_TYPE::TEXTURE];
	constexpr std::string_view ANIM2D = RES_ARR[(int)eRES_TYPE::ANIM2D_ATLAS];
	constexpr std::string_view SOUND = RES_ARR[(int)eRES_TYPE::SOUND];
	constexpr std::string_view PREFAB = RES_ARR[(int)eRES_TYPE::PREFAB];
	

	constexpr std::string_view SHADER_ROOT = "Shader";
	constexpr std::string_view SHADER_GRAPHICS = RES_ARR[(int)eRES_TYPE::GRAPHICS_SHADER];
	constexpr std::string_view SHADER_COMPUTE = RES_ARR[(int)eRES_TYPE::COMPUTE_SHADER];

	constexpr std::string_view SCMAP = "SCMap";
	constexpr std::string_view SCMAP_TILESET = "Tileset";

	//각종 세팅값 저장하는 폴더
	constexpr std::string_view SAVED_SETTING = "SavedSetting";
}


#define JSONKEY(_type) constexpr std::string_view _type = #_type
namespace RES_INFO
{
	namespace JSON_KEY
	{
		JSONKEY(GameObject);
		JSONKEY(eRES_TYPE);
		JSONKEY(strKey);
	}

	namespace MESHDATA
	{
		namespace Ext
		{
			enum class idx
			{
				FBX,
				END
			};
			constexpr std::string_view arr[(int)idx::END] = { ".fbx" };
		}

		constexpr std::string_view DirName = DIRECTORY_NAME::RES_ARR[(int)eRES_TYPE::MESH];
	}

	namespace MATERIAL
	{
		constexpr std::string_view Ext = ".json";

		namespace JSON_KEY
		{
			JSONKEY(strKeyShader);
			JSONKEY(arrStrKeyTex);
			JSONKEY(bUseInstancing);
		}

		constexpr std::string_view DirName = DIRECTORY_NAME::MATERIAL;
	}
	
	//순회 돌 떄 편하게 돌수있도록 배열 형태로 사용
	namespace TEXTURE
	{
		namespace Ext
		{
			enum class idx
			{
				PNG,
				BMP,
				END
			};

			constexpr std::string_view arr[(int)idx::END] = { ".png", ".bmp" };
		}

		constexpr std::string_view DirName = DIRECTORY_NAME::TEXTURE;
	}

	//string strKeyAnim2D;
	//vector<UINT> vecFrame;

	////1차원: 프레임 번호, 2차원: 벡터 내부 콜백 함수
	//vector<vector<std::function<void()>>> vec2D_pFuncCallback;


	////vecFrame.size()와 이 값은 다를 수 있음. 방향 정보에 따라 같은 프레임에 이미지를 보여줘야 할 경우 등등
	//UINT                uNumFrame;
	//float               fFullPlayTime;

	////위의 전체 재생시간 / 프레임 수 한것(한 프레임당 시간) 
	////자동 계산
	//float               fTimePerFrame;

	//eANIM_TYPE          eAnimType;
	//Vec2 vPivot;

	//UINT                uColTotal;
	//UINT                uRowTotal;

	//tAnim2D() : vec2D_pFuncCallback(0), uNumFrame(), fFullPlayTime(), fTimePerFrame(), eAnimType(), uColTotal(), uRowTotal()
	//{}

	namespace ANIM2D
	{
		constexpr std::string_view Ext = ".json";

		namespace JSON_KEY
		{
			JSONKEY(strKeyAtlasTex);
			JSONKEY(vecFrameUV);
			JSONKEY(bRegularFrameSize);

			namespace AnimFrameUV
			{
				JSONKEY(v2_UVLeftTop);
				JSONKEY(v2_UVSlice);
				JSONKEY(v2_Offset);
				JSONKEY(fFullPlayTime);
			}
			
			JSONKEY(mapAnim);
			namespace Anim2D
			{
				JSONKEY(strKeyAnim2D);
				JSONKEY(vecFrame);
				JSONKEY(uNumFrame);
				JSONKEY(fFullPlayTime);
				JSONKEY(eAnimType);
				JSONKEY(vPivot);
				JSONKEY(uColTotal);
				JSONKEY(uRowTotal);
			}

			JSONKEY(uRowTotal);
			JSONKEY(uColTotal);
		}
		
	}

	namespace SOUND
	{
		namespace Ext
		{
			enum class idx
			{
				MP3,
				OGG,
				END
			};
			constexpr std::string_view arr[(int)idx::END] = { ".mp3", ".ogg" };
		}
		constexpr std::string_view DirName = DIRECTORY_NAME::SOUND;
	}

	namespace PREFAB
	{
		constexpr std::string_view Ext = ".json";
		constexpr std::string_view DirName = DIRECTORY_NAME::PREFAB;

		namespace JSON_KEY
		{
			//CComponent* m_arrCom[(UINT)eCOMPONENT_TYPE::END];
			JSONKEY(m_arrCom);

			//CRenderComponent* m_RenderCom;

			//Hierarchy
			//CGameObject* m_Parent;
			//있을 경우 Prefab 형태로 따로 저장해서 불러온 뒤 로드해서 주소를 등록
			JSONKEY(m_Parent_PREFAB);

			//vector<CGameObject*>    m_vecChild;
			JSONKEY(m_vecChild_PREFAB);

			//tMtrlScalarData          m_MtrlScalarData;

			//Layer Info
			//int                     m_iLayerIdx;
			JSONKEY(m_iLayerIdx);

			//레이어 번호를 고정. 부모 레이어를 옮겨도 자신은 옮겨지지 않음.
			//bool                    m_bFixLayer;   
			JSONKEY(m_bFixLayer);

			//Birth, Death
			//bool                    m_bDestroy;

			//float                   m_fLifeSpan;
			JSONKEY(m_fLifeSpan);

			//초기화 되어 현재 Level 안에서 작동중인지 여부를 저장.
			//작동 이후 컴포넌트가 추가될 시 바로 init 호출.
			//bool                m_bInitialized;
			
		}

		namespace COMPONENT
		{
			namespace JSON_KEY
			{
				JSONKEY(eComponentType);
			}
			

			namespace TRANSFORM
			{
				JSONKEY(TRANSFORM);

				//Matrix    ;
				JSONKEY(v3Size);

				//Vec3    m_v3RelativeScale;
				JSONKEY(v3RelativeScale);

				//Vec3    m_v3RelativePos;
				JSONKEY(v3RelativePos);

				//Vec3    m_v3RelativeRot;
				JSONKEY(v3RelativeRot);

				//float m_fLongestDiagonalLen;

				//Vec3    m_vRelativeDir[(UINT)eDIR_TYPE::END];

				//Vec3    m_vWorldDir[(UINT)eDIR_TYPE::END]; 

				//Matrix  m_matRelative;

				//Matrix  m_matParent;

				//bool    m_bInheritScale;
				JSONKEY(bInheritScale);

				//bool    m_bInheritRot;
				JSONKEY(bInheritRot);

				//bool    m_bLockRotation;
				JSONKEY(bLockRot);

				//Matrix  m_matWorldWithoutSize;

				//bool    m_bNeedMyUpdate;

				//bool    m_bNeedParentUpdate;

				//bool    m_bSizeUpdated;
			}

			namespace COLLIDER
			{
				namespace JSON_KEY
				{

					//const eDIMENSION_TYPE     m_eCollDimension;
					JSONKEY(m_eCollDimension);

					//Vec3                m_v2OffsetPos;
					JSONKEY(m_v2OffsetPos);

					//Vec3                m_vOffsetScale;
					JSONKEY(m_vOffsetScale);

					//int                 m_iCollisionCount;
					
				}
			}

			namespace COLLIDER2D
			{
				namespace JSON_KEY
				{
					//eCOLLIDER_TYPE_2D      m_eColType;
					JSONKEY(m_eColType);

					////공간분할에서 검사된 자신이 속한 그리드 인덱스 번호
					//vector<UINT>           m_vecGridIdxInfo;

					////공간분할용 간이 충돌체 정보(직사각형 형태 - 기본 세팅은 transform에서 정사각형 형태)
					////기본 설
					//Vec4                   m_v4SimpleCollLBRTPos;

					////간이충돌체 계산은 각자 충돌체에서 따로 구현해줄것.(점충돌체 때문에)

					////오프셋 위치를 반영한 위치(2D)
					//Vec2                    m_v2CenterPos;
				}

				namespace RECT
				{
					namespace JSON_KEY
					{
						//Vec2 m_v2RectSize;
						JSONKEY(m_v2RectSize);
					}
				}

				namespace CIRCLE
				{
					namespace JSON_KEY
					{
						JSONKEY(m_fRadius);
					}
				}
			}

			namespace COLLIDER3D
			{

			}

			namespace ANIMATOR2D
			{

			}

			namespace ANIMATOR3D
			{

			}

			namespace LIGHT2D
			{

			}

			namespace CAMERA
			{

			}

			namespace RENDER_COMP
			{
				namespace JSON_KEY
				{
					//Ptr<CMesh>              m_pMesh;
					JSONKEY(strKeyMesh);

					//원본 재질. 특별한 상태를 표현할 필요가 없을 경우 이 재질을 사용
					//Ptr<CMaterial>          m_pSharedMtrl;
					JSONKEY(strKeyMtrl);
				}

				namespace MESHRENDER
				{

				}

				namespace PARTICLESYSTEM
				{

				}

				namespace TILEMAP
				{

				}

				namespace LANDSCAPE
				{

				}

				namespace DECAL
				{

				}
			}



			namespace SCRIPT_HOLDER
			{
				namespace JSON_KEY
				{
					JSONKEY(m_vecScript_strKey);
				}
			}
		}
	}


	namespace SHADER
	{
		constexpr std::string_view DirNameRoot = "Shader";
		constexpr std::string_view Ext_ShaderSetting = ".json";
		constexpr std::string_view Ext_ShaderCode = ".cso";

		namespace GRAPHICS
		{
			constexpr std::string_view DirName = DIRECTORY_NAME::SHADER_GRAPHICS;
			constexpr std::string_view arrPrefix[(int)eSHADER_TYPE::END] =
			{ "_V_" , "_H_" , "_D_" ,"_G_" ,"_P_" };

			//각 쉐이더마다 들어가야할 설정값

			namespace JSON_KEY
			{
				//쉐이더 파일의 '이름'만 추출
				//ex) S_1_V_Debug : Debug
				//이름 : CEntity의 Name을 사용
				JSONKEY(eSHADER_PIPELINE_STAGE);
				JSONKEY(D3D_PRIMITIVE_TOPOLOGY);
				JSONKEY(eRASTERIZER_TYPE);
				JSONKEY(eDEPTH_STENCIL_TYPE);
				JSONKEY(eBLEND_STATE_TYPE);
				JSONKEY(eSHADER_DOMAIN);
				JSONKEY(m_bUseInstancing);
			}
		}

		namespace COMPUTE
		{
			constexpr std::string_view DirName = "Compute";
			constexpr std::string_view Prefix = "_C_";

			namespace JSON_KEY
			{
				//쉐이더와 json 파일이 공유하는 파일 이름. 확장자 제외
				//ex) S_C_SCMapLoader.json : S_C_SCMapLoader
				JSONKEY(uarrNumThreadXYZ);
			}
		}
	}


	namespace SCMAP
	{
		constexpr std::string_view DirName = "SCMap";

		namespace Ext
		{
			enum class idx
			{
				SCM,
				SCX,
				END
			};
			constexpr std::string_view arr[(int)idx::END] = { ".scm", ".scx" };
		}
	}




}

