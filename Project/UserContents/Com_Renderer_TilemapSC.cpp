#include "PCH_UserContents.h"
#include "Com_Renderer_TilemapSC.h"

#include "CS_TilemapLoaderSC.h"

#include <EngineBase/Engine/ResourceManager.h>
#include <EngineBase/Engine/PathManager.h>
#include <EngineBase/Engine/StructBuffer.h>

#include "strKey_Shader.h"

namespace ehw
{
    constexpr const char* SCMapDir = "SCMap";
    constexpr const char* SCMapTilesetDir = "Tileset";


    std::array<std::shared_ptr<StructBuffer[]>, (int)scMap::eTilesetInfo::END> Com_Renderer_TilemapSC::s_arrSBuffer_TilesetData{};


    Com_Renderer_TilemapSC::Com_Renderer_TilemapSC()
        : iComponent(eComponentCategory::Renderer)
        , m_arrSBuffer_TilesetData{}
        , m_TilemapLoader{}
        , m_uNumMegatileX{}
        , m_uNumMegatileY{}
        , m_eTileSet{}
        , m_strMapName{}
        , m_vecMegaTile{}
        , m_vecMiniTile{}
        , m_vecUnitData{}
        , m_vecStartLocation{}
        , m_bMapLoaded{}
        , m_bUnitLoaded{}
        , m_pMapTex{}
        , m_SBuffer_MXTM{}
        , m_SBufferRW_Megatile{}
        , m_SBufferRW_Minitile{}
	{
	}
	Com_Renderer_TilemapSC::~Com_Renderer_TilemapSC()
	{
        for (size_t i = 0; i < m_arrSBuffer_TilesetData.size(); ++i)
        {
            m_arrSBuffer_TilesetData[i].reset();

            //공유자원을 사용하는 클래스가 모두 해제되었을 경우(static 자기 자신만 참조중일 경우)
            //static 변수도 해제한다.
            if ((long)1 == s_arrSBuffer_TilesetData[i].use_count())
            {
                s_arrSBuffer_TilesetData[i].reset();
            }
        }


	}
	void Com_Renderer_TilemapSC::Init()
	{
        //컴퓨트쉐이더 로드
		m_TilemapLoader = LOAD_COMPUTESHADER(CS_TilemapLoaderSC);
        ASSERT_DEBUG(m_TilemapLoader, "Tilemap로더 로드되지 않음.");

        if (nullptr == m_arrSBuffer_TilesetData[0].get())
        {
            CreateTilesetData();
        }

        //std::shared_ptr<GraphicsShader> gShader = ResourceManager::Load<GraphicsShader>(strKey_Shader::Graphics::)
        

	}
    void Com_Renderer_TilemapSC::CreateTilesetData()
    {
        std::filesystem::path tilesetDir = PathManager::GetResPathRelative();
        tilesetDir /= SCMapDir;

        ASSERT(std::filesystem::exists(tilesetDir), "타일셋 폴더가 존재하지 않습니다.");

        tilesetDir /= SCMapTilesetDir;

        std::unique_ptr<scMap::tTileset> Tileset = std::make_unique<scMap::tTileset>();

        for (int tilesetIdx = 0; tilesetIdx < (int)scMap::eTilesetInfo::END; ++tilesetIdx)
        {
            //타일셋 데이터 초기화
            memset(Tileset.get(), 0, sizeof(scMap::tTileset));
            
            std::filesystem::path fullPath = tilesetDir;

            //타일셋에 맞는 파일명(문자열)을 추가한다
            fullPath /= scMap::eTilesetInfo_String[tilesetIdx];

            fullPath += ".CV5";
            std::ios::openmode mode = std::ios::beg | std::ios::binary | std::ios::in;

            std::ifstream fpCV5(fullPath, mode);

            ASSERT(fpCV5.is_open(), "타일셋 CV5파일 파일 열기 실패");

            fullPath.replace_extension(".VX4");
            std::ifstream fpVX4(fullPath, mode);
            ASSERT(fpVX4.is_open(), "타일셋 VX4파일 파일 열기 실패");

            fullPath.replace_extension(".VR4");
            std::ifstream fpVR4(fullPath, mode);
            ASSERT(fpVR4.is_open(), "타일셋 VR4파일 파일 열기 실패");

            fullPath.replace_extension(".WPE");
            std::ifstream fpWPE(fullPath, mode);
            ASSERT(fpWPE.is_open(), "타일셋 WPE파일 파일 열기 실패");

            fullPath.replace_extension(".VF4");
            std::ifstream fpVF4(fullPath, mode);
            ASSERT(fpVF4.is_open(), "타일셋 VP4파일 파일 열기 실패");

            //CV5는 모든 데이터를 읽어올 필요가 없다
            UINT16 cpy = (UINT16)0;
            for (int i = 0; i < CV5_MAX; ++i)
            {
                //각각 2바이트씩 읽어와서 캐스트해서 구조체에 넣어준다.
                fpCV5.read((char*)&cpy, sizeof(UINT16));
                Tileset->cv5[i].TerrainType = (UINT32)cpy;
                cpy = 0;

                fpCV5.read((char*)&cpy, sizeof(UINT16));
                Tileset->cv5[i].Flags = (UINT32)cpy;
                cpy = 0;

                //커서를 16만큼 앞으로 이동한뒤
                fpCV5.seekg(16, std::ios::cur);

                //다시 32바이트를 읽는다.
                fpCV5.read((char*)&(Tileset->cv5[i].MegaTileIndex), sizeof(UINT16) * (size_t)16);

                if (true == fpCV5.eof())
                    break;
            }

            //다른 구조체들도 데이터를 읽어온다.
            fpVX4.read((char*)&(Tileset->vx4), sizeof(tVX4) * VX4_MAX);
            fpVR4.read((char*)&(Tileset->vr4), sizeof(tVR4) * VR4_MAX);
            fpWPE.read((char*)&(Tileset->wpe), sizeof(tWPE) * WPE_MAX);
            fpVF4.read((char*)&(Tileset->vf4), sizeof(tVF4) * VF4_MAX);

            //Desc 작성해서 SBuffer 생성
            StructBuffer::Desc SDesc{};
            SDesc.eSBufferType = eStructBufferType::READ_ONLY;
            SDesc.TargetStageSRV = eShaderStageFlag::Compute;

            //타일셋 일괄적으로 동적 할당(2차원 배열)
            s_arrSBuffer_TilesetData[tilesetIdx] = std::make_shared<StructBuffer[]>((int)scMap::eTilesetMember::END);

            for (int i = 0; i < (int)scMap::eTilesetMember::END; ++i)
            {
                //0 ~ 5번까지 일치시켜 놓았음.
                SDesc.REGISLOT_t_SRV = i;

                //Desc 설정
                m_arrSBuffer_TilesetData[tilesetIdx][i].SetDesc(SDesc);

                //각자에게 맞는 구조화버퍼 공간 생성
                switch ((scMap::eTilesetMember)i)
                {
                case scMap::eTilesetMember::CV5:
                    m_arrSBuffer_TilesetData[tilesetIdx][i].Create((UINT)sizeof(tCV5), (UINT)CV5_MAX, Tileset->cv5, (UINT)CV5_MAX);

                    break;
                case scMap::eTilesetMember::VX4:
                    m_arrSBuffer_TilesetData[tilesetIdx][i].Create((UINT)sizeof(tVX4), (UINT)VX4_MAX, Tileset->vx4, (UINT)VX4_MAX);

                    break;
                case scMap::eTilesetMember::VF4:
                    m_arrSBuffer_TilesetData[tilesetIdx][i].Create((UINT)sizeof(tVF4), (UINT)VF4_MAX, Tileset->vf4, (UINT)VF4_MAX);

                    break;
                case scMap::eTilesetMember::VR4:
                    m_arrSBuffer_TilesetData[tilesetIdx][i].Create((UINT)sizeof(tVR4), (UINT)VR4_MAX, Tileset->vr4, (UINT)VR4_MAX);

                    break;
                case scMap::eTilesetMember::WPE:
                    m_arrSBuffer_TilesetData[tilesetIdx][i].Create((UINT)sizeof(tWPE), (UINT)WPE_MAX, Tileset->wpe, (UINT)WPE_MAX);

                    break;

                default:
                    break;
                }
            }
        }
    }
}
