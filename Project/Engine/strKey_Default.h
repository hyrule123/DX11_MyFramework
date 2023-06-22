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

namespace strKey_RES_DEFAULT
{
	namespace MESH
	{
		STRKEY POINT = "PointMesh";
		STRKEY RECT = "RectMesh";
		STRKEY CIRCLE = "CircleMesh";

		STRKEY DEBUG_RECT = "RectMesh_Debug";
		STRKEY DEBUG_CIRCLE = "CircleMesh_Debug";
	}


	namespace MATERIAL
	{
		STRKEY DEBUG_RECT = "DebugRect.json";
		STRKEY DEBUG_CIRCLE = "DebugCircle.json";

		STRKEY TEST = "Test.json";
		STRKEY STD2D = "std2D.json";
		STRKEY STD2D_LIGHT = "std2DLight.json";
		
		STRKEY TILEMAP_SC = "TilemapComplete.json";
		STRKEY TILEMAP_ATLAS = "TilemapAtlas.json";

		STRKEY PARTICLE_RENDER = "ParticleRender.json";
	}

	namespace SHADER
	{
		namespace GRAPHICS
		{
			STRKEY DEBUG = "Debug.json";
			STRKEY PARTICLE = "Particle.json";
			STRKEY STD2D = "STD2D.json";
			STRKEY STD2DLIGHT = "STD2DLight.json";
			STRKEY TEST = "Test.json";
			STRKEY TILEMAPATLAS = "TilemapAtlas.json";
			STRKEY TILEMAPCOMPLETE = "TilemapComplete.json";
		}

		namespace COMPUTE
		{
			STRKEY SCMAPLOADER = "S_C_SCMapLoader.json";
			STRKEY INITALIZE = "S_C_Initalize.json";
			STRKEY PARTICLEBASIC = "S_C_ParticleBasic.json";
			STRKEY PARTICLERAINDROP = "S_C_ParticleRainDrop.json";
			STRKEY SETCOLOR = "S_C_SetColor.json";
		}
	}
	
	namespace TEXTURE
	{
		STRKEY DEFAULT = "Default.png";
		STRKEY NOISE_1 = "Noise/noise_01.png";
	}
}

namespace DIRECTORY_NAME
{
	//인덱스는 eRES_TYPE를 사용
	//ResMgr에서 관리되는 리소스의 키 및 폴더 이름으로 사용
	STRKEY RES_ARR[(int)eRES_TYPE::END] =
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
	STRKEY CONFIGURATION = "_Debug";
#else
	STRKEY CONFIGURATION = "_Release";
#endif

	STRKEY RELATIVE_BEGIN = ".";
	STRKEY CONTENT = "Content";
	STRKEY MESHDATA = RES_ARR[(int)eRES_TYPE::MESHDATA];
	STRKEY MATERIAL = RES_ARR[(int)eRES_TYPE::MATERIAL];
	STRKEY TEXTURE = RES_ARR[(int)eRES_TYPE::TEXTURE];
	STRKEY ANIM2D = RES_ARR[(int)eRES_TYPE::ANIM2D_ATLAS];
	STRKEY SOUND = RES_ARR[(int)eRES_TYPE::SOUND];
	STRKEY PREFAB = RES_ARR[(int)eRES_TYPE::PREFAB];
	

	STRKEY SHADER_ROOT = "Shader";
	STRKEY SHADER_GRAPHICS = RES_ARR[(int)eRES_TYPE::GRAPHICS_SHADER];
	STRKEY SHADER_COMPUTE = RES_ARR[(int)eRES_TYPE::COMPUTE_SHADER];

	STRKEY SCMAP = "SCMap";
	STRKEY SCMAP_TILESET = "Tileset";

	//각종 세팅값 저장하는 폴더
	STRKEY SAVED_SETTING = "SavedSetting";
}



namespace RES_INFO
{
	namespace JSON_KEY
	{
		STRKEY_DECLARE(GameObject);
		STRKEY_DECLARE(eRES_TYPE);
		STRKEY_DECLARE(strKey);
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
			STRKEY arr[(int)idx::END] = { ".fbx" };
		}

		STRKEY DirName = DIRECTORY_NAME::RES_ARR[(int)eRES_TYPE::MESH];
	}

	namespace MATERIAL
	{
		STRKEY Ext = ".json";

		namespace JSON_KEY
		{
			STRKEY_DECLARE(strKey_Shader);
			STRKEY_DECLARE(arrStrKeyTex);
			STRKEY_DECLARE(bUseInstancing);
		}

		STRKEY DirName = DIRECTORY_NAME::MATERIAL;
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

			STRKEY arr[(int)idx::END] = { ".png", ".bmp" };
		}

		STRKEY DirName = DIRECTORY_NAME::TEXTURE;
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
		STRKEY Ext = ".json";

		namespace JSON_KEY
		{
			STRKEY_DECLARE(strKeyAtlasTex);
			STRKEY_DECLARE(vecFrameUV);
			STRKEY_DECLARE(bRegularFrameSize);

			namespace AnimFrameUV
			{
				STRKEY_DECLARE(v2_UVLeftTop);
				STRKEY_DECLARE(v2_UVSlice);
				STRKEY_DECLARE(v2_Offset);
				STRKEY_DECLARE(fFullPlayTime);
			}
			
			STRKEY_DECLARE(mapAnim);
			namespace Anim2D
			{
				STRKEY_DECLARE(strKeyAnim2D);
				STRKEY_DECLARE(vecFrame);
				STRKEY_DECLARE(uNumFrame);
				STRKEY_DECLARE(fFullPlayTime);
				STRKEY_DECLARE(eAnimType);
				STRKEY_DECLARE(vPivot);
				STRKEY_DECLARE(uColTotal);
				STRKEY_DECLARE(uRowTotal);
			}

			STRKEY_DECLARE(uRowTotal);
			STRKEY_DECLARE(uColTotal);
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
			STRKEY arr[(int)idx::END] = { ".mp3", ".ogg" };
		}
		STRKEY DirName = DIRECTORY_NAME::SOUND;
	}

	namespace PREFAB
	{
		STRKEY Ext = ".json";
		STRKEY DirName = DIRECTORY_NAME::PREFAB;

		namespace JSON_KEY
		{
			STRKEY_DECLARE(m_Transform);

			//CComponent* m_arrCom[(UINT)eCOMPONENT_TYPE::END];
			STRKEY_DECLARE(m_arrCom);

			//CRenderComponent* m_RenderCom;

			//Hierarchy
			//CGameObject* m_Parent;
			//있을 경우 Prefab 형태로 따로 저장해서 불러온 뒤 로드해서 주소를 등록
			STRKEY_DECLARE(m_Parent_PREFAB);

			//vector<CGameObject*>    m_vecChild;
			STRKEY_DECLARE(m_vecChild_PREFAB);

			//tMtrlScalarData          m_MtrlScalarData;

			//Layer Info
			//int                     m_iLayerIdx;
			STRKEY_DECLARE(m_iLayerIdx);

			//레이어 번호를 고정. 부모 레이어를 옮겨도 자신은 옮겨지지 않음.
			//bool                    m_bFixLayer;   
			STRKEY_DECLARE(m_bFixLayer);

			//Birth, Death
			//bool                    m_bDestroy;

			//float                   m_fLifeSpan;
			STRKEY_DECLARE(m_fLifeSpan);

			//초기화 되어 현재 Level 안에서 작동중인지 여부를 저장.
			//작동 이후 컴포넌트가 추가될 시 바로 init 호출.
			//bool                m_bInitialized;
			
		}

		namespace COMPONENT
		{
			namespace JSON_KEY
			{
				STRKEY_DECLARE(eComponentType);
			}
			

			namespace TRANSFORM
			{
				STRKEY_DECLARE(TRANSFORM);

				//Matrix    ;
				STRKEY_DECLARE(v3Size);

				//Vec3    m_v3RelativeScale;
				STRKEY_DECLARE(v3RelativeScale);

				//Vec3    m_v3RelativePos;
				STRKEY_DECLARE(v3RelativePos);

				//Vec3    m_v3RelativeRot;
				STRKEY_DECLARE(v3RelativeRot);

				//float m_fLongestDiagonalLen;

				//Vec3    m_vRelativeDir[(UINT)eDIR_TYPE::END];

				//Vec3    m_vWorldDir[(UINT)eDIR_TYPE::END]; 

				//Matrix  m_matRelative;

				//Matrix  m_matParent;

				//bool    m_bInheritScale;
				STRKEY_DECLARE(bInheritScale);

				//bool    m_bInheritRot;
				STRKEY_DECLARE(bInheritRot);

				//bool    m_bLockRotation;
				STRKEY_DECLARE(bLockRot);

				//Matrix  m_matWorldWithoutSize;

				//bool    m_bNeedMyUpdate;

				//bool    m_bNeedParentUpdate;

				//bool    m_bSizeUpdated;
			}

			namespace COLLIDER
			{
				namespace JSON_KEY
				{
					//Position은 무조건 트랜스폼을 따라감. 이 값은 Transform의 위치값에 추가로 Offset을 줌.
					STRKEY_DECLARE(m_v3OffsetPos);

					STRKEY_DECLARE(m_bFollowTransformSize);
					STRKEY_DECLARE(m_v3Size);
				}
			}

			namespace COLLIDER2D
			{
				namespace JSON_KEY
				{
					//eCOLLIDER_TYPE_2D      m_eColType;
					STRKEY_DECLARE(m_eColType);

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
						STRKEY_DECLARE(m_v2RectSize);
					}
				}

				namespace CIRCLE
				{
					namespace JSON_KEY
					{
						STRKEY_DECLARE(m_fRadius);
					}
				}
			}

			namespace COLLIDER3D
			{

			}

			namespace ANIMATOR2D
			{
				namespace JSON_KEY
				{
					STRKEY_DECLARE(m_arrAtlasTex);
				}
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
					STRKEY_DECLARE(strKeyMesh);

					//원본 재질. 특별한 상태를 표현할 필요가 없을 경우 이 재질을 사용
					//Ptr<CMaterial>          m_pSharedMtrl;
					STRKEY_DECLARE(strKeyMtrl);
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
					STRKEY_DECLARE(m_vecScript_strKey);
				}
			}
		}
	}


	namespace SHADER
	{
		STRKEY DirNameRoot = "Shader";
		STRKEY Ext_ShaderSetting = ".json";
		STRKEY Ext_ShaderCode = ".cso";

		namespace GRAPHICS
		{
			STRKEY DirName = DIRECTORY_NAME::SHADER_GRAPHICS;
			STRKEY arrPrefix[(int)def_Shader::eSHADER_TYPE::END] =
			{ "_V_" , "_H_" , "_D_" ,"_G_" ,"_P_" };

			//각 쉐이더마다 들어가야할 설정값

			namespace JSON_KEY
			{
				//쉐이더 파일의 '이름'만 추출
				//ex) S_1_V_Debug : Debug
				//이름 : CEntity의 Name을 사용
				//자동 생성된 쉐이더 파이프라인의 경우 비트마스크를 사용해서 로드하고
				//직접 만든 쉐이더 파이프라인의 경우 파일명을 통해서 로드한다.
				//STRKEY_DECLARE(eSHADER_PIPELINE_STAGE);
				STRKEY_DECLARE(_bIsAutoGenerated);

				STRKEY_DECLARE(D3D_PRIMITIVE_TOPOLOGY);
				STRKEY_DECLARE(eRASTERIZER_TYPE);
				STRKEY_DECLARE(eDEPTH_STENCIL_TYPE);
				STRKEY_DECLARE(eBLEND_STATE_TYPE);
				STRKEY_DECLARE(eSHADER_DOMAIN);
				STRKEY_DECLARE(m_bUseInstancing);


				constexpr inline const char* arr_strKey_PipeLine[(int)def_Shader::eSHADER_TYPE::END] =
				{
					"_0_VERTEX",
					"_1_HULL",
					"_2_DOMAIN",
					"_3_GEOMETRY",
					"_4_PIXEL"
				};
			}
		}

		namespace COMPUTE
		{
			STRKEY DirName = "Compute";
			STRKEY Prefix = "_C_";

			namespace JSON_KEY
			{
				//쉐이더와 json 파일이 공유하는 파일 이름. 확장자 제외
				//ex) S_C_SCMapLoader.json : S_C_SCMapLoader
				STRKEY_DECLARE(uarrNumThreadXYZ);
			}
		}
	}


	namespace SCMAP
	{
		STRKEY DirName = "SCMap";

		namespace Ext
		{
			enum class idx
			{
				SCM,
				SCX,
				END
			};
			STRKEY arr[(int)idx::END] = { ".scm", ".scx" };
		}
	}




}

