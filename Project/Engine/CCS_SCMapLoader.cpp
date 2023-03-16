#include "pch.h"
#include "CCS_SCMapLoader.h"


//#ifdef _DEBUG
//#pragma comment (lib, "StormLib_DLL/StormLib_DLL_Debug")
//#else
//#pragma comment (lib, "StormLib_DLL/StormLib_DLL")
//#endif

//#define STORMLIB_NO_AUTO_LINK
//#include <StormLib_DLL/StormLib.h>


//일단 여기서 몰아서 성공킬예정
//나중에 지우고 분산시킬것
#include "CPathMgr.h"
#include "CStructBuffer.h"
#include "CTexture.h"
#include "CResMgr.h"

#ifdef _DEBUG
constexpr const wchar_t* strStormLibPathW = L"StormLib_DLL_Debug.dll";
constexpr const char* strStormLibPathA = "StormLib_DLL_Debug.dll";
#else
constexpr const wchar_t* strStormLibPathW = L"StormLib_DLL_Release.dll";
constexpr const char* strStormLibPathA = "StormLib_DLL_Release.dll";
#endif

//맵 정보 읽기용
constexpr const char* TerrainChunk = "ERA";
constexpr const char* MapSizeChunk = "DIM";
constexpr const char* TileMapChunk = "MTXM";

CCS_SCMapLoader::CCS_SCMapLoader()
	: CComputeShader(32u, 32u, 1u)	//메가타일 사이즈 = 32 * 32
    , m_arrpSBufferTileSet{}
    , m_pSBuffer_MXTM()
    , m_pSBuffer_Debug()
    , m_DebugData()
    
{
    //wstring Path = CPathMgr::GetInst()->GetContentAbsPathW();
    std::filesystem::path Path(RELATIVE_PATH::SCMAP::A);
    Path /= "Tilesets";


    //타일셋 데이터를 저장할 메모리공간 동적할당 
    tTileSet* Tileset = new tTileSet;
    for (UINT8 TileSetIdx = (UINT8)0u; TileSetIdx < (UINT8)eTILESET_INFO::END; ++TileSetIdx)
    {
        //타일셋 데이터 초기화
        memset(Tileset, 0, sizeof(tTileSet));
        std::filesystem::path FullPath = Path;

        switch ((eTILESET_INFO)TileSetIdx)
        {
        case eTILESET_INFO::BADLANDS:
            FullPath /= "badlands";
            break;
        case eTILESET_INFO::SPACE_PLATFORM:
            FullPath /= "platform";
            break;
        case eTILESET_INFO::INSTALLATION:
            FullPath /= "install";
            break;
        case eTILESET_INFO::ASH_WORLD:
            FullPath /= "ashworld";
            break;
        case eTILESET_INFO::JUNGLE:
            FullPath /= "jungle";
            break;
        case eTILESET_INFO::DESERT:
            FullPath /= "Desert";
            break;
        case eTILESET_INFO::ICE:
            FullPath /= "Ice";
            break;
        case eTILESET_INFO::TWILIGHT:
            FullPath /= "Twilight";
            break;
        default:
            break;
        }

        FILE* fpCV5, * fpVX4, * fpVR4, * fpWPE, * fpVF4;
        FullPath += ".CV5";
        _wfopen_s(&fpCV5, FullPath.wstring().c_str(), L"rb");

        FullPath.replace_extension(".VX4");
        _wfopen_s(&fpVX4, FullPath.wstring().c_str(), L"rb");

        FullPath.replace_extension(".VR4");
        _wfopen_s(&fpVR4, FullPath.wstring().c_str(), L"rb");

        FullPath.replace_extension(".WPE");
        _wfopen_s(&fpWPE, FullPath.wstring().c_str(), L"rb");

        FullPath.replace_extension(".VF4");
        _wfopen_s(&fpVF4, FullPath.wstring().c_str(), L"rb");


        if (fpCV5 == nullptr || fpVX4 == nullptr || fpVR4 == nullptr || fpWPE == nullptr || fpVF4 == nullptr)
        {
            _fcloseall();
            delete Tileset;
            MessageBoxA(nullptr, "Tileset Load Failed", nullptr, MB_OK);
            assert(nullptr);
        }



        for (int i = 0; i < CV5_MAX; ++i)
        {
            //매회 20만큼 이동해서 더미 데이터를 버리고 필요한 32(sizeof(CV5))만큼 가져옴.
            fseek(fpCV5, 20, SEEK_CUR);
            size_t BytesRead = fread(&(Tileset->cv5[i]), sizeof(CV5), 1, fpCV5);

            if ((size_t)0u == BytesRead)
                break;
        }

        fread(Tileset->vx4, sizeof(VX4), VX4_MAX, fpVX4);
        fread(Tileset->vr4, sizeof(VR4), VR4_MAX, fpVR4);
        fread(Tileset->wpe, sizeof(WPE), WPE_MAX, fpWPE);
        fread(Tileset->vf4, sizeof(VF4), VF4_MAX, fpVF4);

        //Desc 작성해서 SBuffer 생성
        tSBufferDesc SDesc = { eSTRUCT_BUFFER_TYPE::READ_ONLY,
        eSHADER_PIPELINE_STAGE::__COMPUTE,
        eCBUFFER_SBUFFER_SHAREDATA_IDX::NONE,
        idx_t_SRV_NONE,
        idx_u_UAV_NONE
        };
        //타일셋 일괄적으로 동적 할당
        

        m_arrpSBufferTileSet[TileSetIdx].arrTileSetMember = new CStructBuffer[(int)eTILESET_MEMBER::END];
        for (int i = 0; i < (int)eTILESET_MEMBER::END; ++i)
        {
            //0 ~ 5번까지 일치시켜 놓았음.
            SDesc.i_idx_t_SRVIdx = i;

            //Desc 설정
            m_arrpSBufferTileSet[TileSetIdx].arrTileSetMember[i].SetDesc(SDesc);

            //각자에게 맞는 구조화버퍼 공간 생성
            switch ((eTILESET_MEMBER)i)
            {
            case eTILESET_MEMBER::CV5:
                m_arrpSBufferTileSet[TileSetIdx].arrTileSetMember[i].Create((UINT)sizeof(CV5), (UINT)CV5_MAX, Tileset->cv5, (UINT)CV5_MAX);
                
                break;
            case eTILESET_MEMBER::VX4:
                m_arrpSBufferTileSet[TileSetIdx].arrTileSetMember[i].Create((UINT)sizeof(VX4), (UINT)VX4_MAX, Tileset->vx4, (UINT)VX4_MAX);

                break;
            case eTILESET_MEMBER::VF4:
                m_arrpSBufferTileSet[TileSetIdx].arrTileSetMember[i].Create((UINT)sizeof(VF4), (UINT)VF4_MAX, Tileset->vf4, (UINT)VF4_MAX);

                break;
            case eTILESET_MEMBER::VR4:
                m_arrpSBufferTileSet[TileSetIdx].arrTileSetMember[i].Create((UINT)sizeof(VR4), (UINT)VR4_MAX, Tileset->vr4, (UINT)VR4_MAX);

                break;
            case eTILESET_MEMBER::WPE:
                m_arrpSBufferTileSet[TileSetIdx].arrTileSetMember[i].Create((UINT)sizeof(WPE), (UINT)WPE_MAX, Tileset->wpe, (UINT)WPE_MAX);

                break;

            default:
                break;
            }
        }


        _fcloseall();
    }

    delete Tileset;
}

CCS_SCMapLoader::~CCS_SCMapLoader()
{
    for (int i = 0; i < (int)eTILESET_INFO::END; ++i)
    {
        DESTRUCTOR_DELETE_ARRAY(m_arrpSBufferTileSet[i].arrTileSetMember);
    }

    DESTRUCTOR_DELETE(m_pSBuffer_MXTM);


    DESTRUCTOR_DELETE(m_pSBuffer_Debug);
    if (nullptr != m_DebugData)
        delete[] m_DebugData;
    
}

bool CCS_SCMapLoader::BindDataCS()
{
    //wstring MapPath = CPathMgr::GetInst()->GetContentAbsPathW();

    static std::filesystem::path MapDir = RELATIVE_PATH::SCMAP::A;
    
    std::filesystem::path MapPath = MapDir / m_tMapWorkSpace.strMapName;

    

    HANDLE hMpq = NULL;          // Open archive handle
    HANDLE hFile = NULL;          // Archived file handle
    //HANDLE handle = NULL;          // Disk file handle

    //DLL 로드
    HMODULE DLLModule = LoadLibraryA(strStormLibPathA);
    if (nullptr == DLLModule)
        return false;

    //함수 등록
    //MPQ 파일 로드
    typedef bool(WINAPI* pSFileOpenArchive)(const TCHAR*, DWORD, DWORD, HANDLE*);
    pSFileOpenArchive MpqOpenFunc = (pSFileOpenArchive)GetProcAddress(DLLModule, "SFileOpenArchive");
    
    typedef bool(WINAPI* pSFileOpenFileEx)(HANDLE, const char*, DWORD, HANDLE*);
    pSFileOpenFileEx funcOpenScenarioChk = (pSFileOpenFileEx)GetProcAddress(DLLModule, "SFileOpenFileEx");

    //DWORD WINAPI SFileGetFileSize(HANDLE hFile, LPDWORD pdwFileSizeHigh);
    typedef DWORD(WINAPI* pSFileGetFileSize)(HANDLE, LPDWORD);
    pSFileGetFileSize funcGetFileSize = (pSFileGetFileSize)GetProcAddress(DLLModule, "SFileGetFileSize");
    
    //bool WINAPI SFileReadFile(HANDLE hFile, void* pvBuffer, DWORD dwToRead, LPDWORD pdwRead, LPOVERLAPPED lpOverlapped);
    typedef bool (*pSFileReadFile)(HANDLE, void*, DWORD, LPDWORD, LPOVERLAPPED);
    pSFileReadFile funcReadFile = (pSFileReadFile)GetProcAddress(DLLModule, "SFileReadFile");

    //bool WINAPI SFileCloseFile(HANDLE hFile)
    typedef bool (WINAPI* pSFileCloseFile)(HANDLE);
    pSFileCloseFile funcCloseFile = (pSFileCloseFile)GetProcAddress(DLLModule, "SFileCloseFile");

    //bool WINAPI SFileCloseArchive(HANDLE hMpq)
    typedef bool (WINAPI* pSFileCloseArchive)(HANDLE);
    pSFileCloseArchive funcCloseArchive = (pSFileCloseArchive)GetProcAddress(DLLModule, "SFileCloseArchive");


    //DLL로부터 로드 실패 또는 함수에서 로드를 실패했을 경우 false 반환
    if (nullptr == MpqOpenFunc || false == MpqOpenFunc(MapPath.c_str(), 0, 0, &hMpq))
    {
        int nError = GetLastError();
        return false;
    }
        
    //MPQ 파일 내부의 scenario.chk 파일 탐색.
    if (nullptr == funcOpenScenarioChk || false == funcOpenScenarioChk(hMpq, "staredit\\scenario.chk", 0, &hFile))
    {
        int nError = GetLastError();
        return false;
    }
        

    // Read the file from the archive
    char* szBuffer;
    DWORD dwBytes = 1;

    //파일의 사이즈를 받아온 후 해당 파일을 받아올 공간을 동적할당 및 초기화한다.
    DWORD FileSize = funcGetFileSize(hFile, NULL);
    szBuffer = new char[FileSize];
    memset(szBuffer, 0, (size_t)FileSize);


    //데이터를 읽어온다.
    funcReadFile(hFile, szBuffer, FileSize, &dwBytes, NULL);

    
    //파일 연결 해제
    funcCloseFile(hFile);
    funcCloseArchive(hMpq);
    
    
    
    //라이브러리 연결 해제
    FreeLibrary(DLLModule);


    //szbuffer에 맵의 데이터가 들어와있을 것임.
    //여기서 맵을 그려내는데 필요한 정보를 가져와야 함
    bool bLoaded = ReadMapData(szBuffer, FileSize);

    delete[] szBuffer;

    //맵데이터를 읽어오는것까지 성공했을 경우 CS로 데이터를 업로드한다.
    if (true == bLoaded)
        bLoaded = UploadMapDataToCS();

	return bLoaded;



    //암시적 링킹 코드
//        // Open an archive, e.g. "d2music.mpq"
//    if (nError == ERROR_SUCCESS)
//    {
//        if (!SFileOpenArchive(MapPath.c_str(), 0, 0, &hMpq))
//            nError = GetLastError();
//    }
//
//    typedef  bool (WINAPI* SFileOpenIndex)(HANDLE, const char*, DWORD, HANDLE);
//
//    // Open a file in the archive, e.g. "data\global\music\Act1\tristram.wav"
//    if (nError == ERROR_SUCCESS)
//    {
//        //맵 파일의 scenario.chk 데이터를 찾는다.
//        if (!SFileOpenFileEx(hMpq, "staredit\\scenario.chk", 0, &hFile))
//            nError = GetLastError();
//    }
//
//    if (nError != ERROR_SUCCESS)
//        return false;
//
//    // Read the file from the archive
//    char* szBuffer;
//    DWORD dwBytes = 1;
//
//    //찾은 scenario.chk 파일로부터 정보를 읽어온다.
//    DWORD FileSize = SFileGetFileSize(hFile, NULL);
//
//    szBuffer = new char[FileSize];
//
//    SFileReadFile(hFile, szBuffer, FileSize, &dwBytes, NULL);
//
//    // Cleanup and exit
////if (handle != NULL)
////    CloseHandle(handle);
//    if (hFile != NULL)
//        SFileCloseFile(hFile);
//    if (hMpq != NULL)
//        SFileCloseArchive(hMpq);
}

void CCS_SCMapLoader::UnBindCS()
{
    //타일셋 Unbind
    for (int i = 0; i < (int)eTILESET_MEMBER::END; ++i)
    {
        m_arrpSBufferTileSet[(int)m_tMapWorkSpace.eTileSet].arrTileSetMember[i].UnBind();
    }
 
    //맵 정보는 제거
    SAFE_DELETE(m_pSBuffer_MXTM);

    //Walkability 데이터 가져온 뒤 제거
    UINT numMinitile = m_tMapWorkSpace.uMapSizeX * m_tMapWorkSpace.uMapSizeY * 16;
    m_pSBufferRW_Walkability->GetData(m_tMapWorkSpace.vecWalkabilityMap.data(), sizeof(tWalkability) * numMinitile);
    SAFE_DELETE(m_pSBufferRW_Walkability);

    m_tMapWorkSpace.pMapTex->UnBind();

    Debug();
}

void CCS_SCMapLoader::Debug()
{
    //m_pSBuffer_Debug->GetData(m_DebugData, sizeof(tMtrlScalarData) * m_MapSizeX * m_MapSizeY);

    //for (int i = 0; i < m_MapSizeX * m_MapSizeY; ++i)
    //{
    //    tMtrlScalarData Temp = m_DebugData[i];

    //    int a = 0;
    //}
}

bool CCS_SCMapLoader::LoadMap(const string& _strMapName, __out tMapData& _tMapData)
{
    _tMapData = tMapData();
    m_tMapWorkSpace.strMapName = _strMapName;

    //로드 실패 시 들어온 레퍼런스를 초기화하고 false 반환
    if (false == Execute())
        return false;
            
    //성공 시 작업 정보를 넘겨주고 자신의 작업 영역은 초기화한다.
    _tMapData = m_tMapWorkSpace;
    CResMgr::GetInst()->AddRes<CTexture>(_tMapData.strMapName, m_tMapWorkSpace.pMapTex);
    m_tMapWorkSpace = tMapData();
    return true;
}


bool CCS_SCMapLoader::ReadMapData(char* Data, DWORD Size)
{
    //tMapDataChunk 자체의 데이터 사이즈는 크지 않으므로 스택에 생성
    tMapDataChunk arrMapDataChunk[(int)eSCMAP_DATA_TYPE::END] = {};

    bool bLoaded = false;
    do
    {
        //읽어온 데이터를 string에 옮긴다.
        std::string_view View(Data, Size);
        string DataStr(View);


        //Terrain tMapDataChunk 시작점(문자열) 을 찾는다.
        size_t pos = 0;
        pos = DataStr.find(TerrainChunk);

        if (pos == string::npos)
            break;

        {
            tMapDataChunk* chk = &arrMapDataChunk[(int)eSCMAP_DATA_TYPE::TERRAIN];

            char* adress = Data + pos;

            //이름 복사
            memcpy(chk->TypeName, adress, 4);

            //데이터 덩어리 사이즈 복사
            int* len = (int*)(adress + 4);
            chk->length = *len;

            chk->Data = new unsigned char[chk->length + 1];
            memcpy(chk->Data, adress + 8, chk->length);
        }

        pos = 0;
        pos = DataStr.find(MapSizeChunk);

        if (pos == std::string::npos)
            break;
                   
        {
            tMapDataChunk* chk = &arrMapDataChunk[(int)eSCMAP_DATA_TYPE::MAPSIZE];

            char* adress = Data + pos;

            //이름 복사
            memcpy(chk->TypeName, adress, 4);

            //데이터 덩어리의 사이즈 복사
            int* len = (int*)(adress + 4);
            chk->length = *len;

            chk->Data = new unsigned char[chk->length + 1];
            memcpy(chk->Data, adress + 8, chk->length);            
        }


        pos = 0;
        pos = DataStr.find(TileMapChunk);

        if (pos == std::string::npos)
            break;

        {
            tMapDataChunk* chk = &arrMapDataChunk[(int)eSCMAP_DATA_TYPE::TILEMAP_ATLAS];

            char* adress = Data + pos;

            //이름 복사
            memcpy(chk->TypeName, adress, 4);

            //데이터 덩어리의 사이즈 복사
            int* len = (int*)(adress + 4);
            chk->length = *len;

            chk->Data = new unsigned char[chk->length + 1];
            memcpy(chk->Data, adress + 8, chk->length);
        }

        bLoaded = true;

    } while (false);

    if (false == bLoaded)
        return false;


    //맵 사이즈 읽기
    if (arrMapDataChunk[(int)eSCMAP_DATA_TYPE::MAPSIZE].length != 4)
        return false;

    m_tMapWorkSpace.uMapSizeX = (int)*(unsigned short*)(arrMapDataChunk[(int)eSCMAP_DATA_TYPE::MAPSIZE].Data);
    m_tMapWorkSpace.uMapSizeY = (int)*(unsigned short*)(arrMapDataChunk[(int)eSCMAP_DATA_TYPE::MAPSIZE].Data + 2);

    Vec2 vMapSize = Vec2(m_tMapWorkSpace.uMapSizeX, m_tMapWorkSpace.uMapSizeY);
    SetMtrlScalarParam(MTRL_SCALAR_VEC2_MAPSIZE, &vMapSize);

    //MXTM(TILEMAP_ATLAS) 생성 및 전송
    SAFE_DELETE(m_pSBuffer_MXTM);
    tSBufferDesc SBufferDesc = { eSTRUCT_BUFFER_TYPE::READ_ONLY, eSHADER_PIPELINE_STAGE::__COMPUTE, eCBUFFER_SBUFFER_SHAREDATA_IDX::NONE, idx_t_SBUFFER_MXTM, idx_u_UAV_NONE };
   
    m_pSBuffer_MXTM = new CStructBuffer(SBufferDesc);
    UINT DataCount = (UINT)arrMapDataChunk[(int)eSCMAP_DATA_TYPE::TILEMAP_ATLAS].length / 16u;
    m_pSBuffer_MXTM->Create((UINT)sizeof(MXTM), DataCount, arrMapDataChunk[(int)eSCMAP_DATA_TYPE::TILEMAP_ATLAS].Data, DataCount);
    m_pSBuffer_MXTM->BindBufferSRV();


    //지형 파일 읽기
    if (arrMapDataChunk[(int)eSCMAP_DATA_TYPE::TERRAIN].length != 2)
        return false;

    //첫 1바이트에 지형 정보가 들어있다.
    unsigned char Info = arrMapDataChunk[(int)eSCMAP_DATA_TYPE::TERRAIN].Data[0];

    //앞의 4비트는 0으로 변환
    unsigned char bitshift = 0b00001111;
    Info &= bitshift;

    //혹시나 비트시프트 이후 8을 초과하는 값이 나올 경우 8을 빼준다.
    if (Info >= (unsigned char)8)
        Info -= (unsigned char)8;

    m_tMapWorkSpace.eTileSet = static_cast<eTILESET_INFO>(Info);

    return true;
}

bool CCS_SCMapLoader::UploadMapDataToCS()
{
    //타일셋을 바인딩해준다.
    for (int i = 0; i < (int)eTILESET_MEMBER::END; ++i)
    {
        m_arrpSBufferTileSet[(int)m_tMapWorkSpace.eTileSet].arrTileSetMember[i].BindBufferSRV();
    }

    //Walkability map을 보내고 받아올 구조화 버퍼를 생성한다.
    tSBufferDesc Desc = {};
    Desc.eCBufferIdx = eCBUFFER_SBUFFER_SHAREDATA_IDX::NONE;
    Desc.eSBufferType = eSTRUCT_BUFFER_TYPE::READ_WRITE;
    Desc.flag_eSHADER_PIPELINE_STAGE_FLAG_SRV = eSHADER_PIPELINE_STAGE::__COMPUTE;
    Desc.i_idx_t_SRVIdx = idx_t_SRV_NONE;
    Desc.i_idx_u_UAVIdx = idx_u_SBUFFERRW_WALKABILITY;


    SAFE_DELETE(m_pSBufferRW_Walkability);
    m_pSBufferRW_Walkability = new CStructBuffer(Desc);

    //메가타일 하나당 16개의 미니타일이 존재
    UINT numMinitile = m_tMapWorkSpace.uMapSizeX * m_tMapWorkSpace.uMapSizeY * 16;
    m_pSBufferRW_Walkability->Create(sizeof(tWalkability), numMinitile, nullptr, 0u);
    m_pSBufferRW_Walkability->BindBufferUAV();

    //데이터를 받아올 vector도 리사이즈 미리 해놓음.
    m_tMapWorkSpace.vecWalkabilityMap.resize(numMinitile);
    

    //타겟이 될 텍스처를 동적할당하고, UAV에 바인딩
    m_tMapWorkSpace.pMapTex = new CTexture;
    UINT BindFlag = D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
    m_tMapWorkSpace.pMapTex->Create(32u * m_tMapWorkSpace.uMapSizeX, 32u * m_tMapWorkSpace.uMapSizeY, DXGI_FORMAT_B8G8R8A8_UNORM, BindFlag, D3D11_USAGE::D3D11_USAGE_DEFAULT);
    m_tMapWorkSpace.pMapTex->BindData_UAV(idx_u_TEXTURERW_TARGET);

    //맵 이름으로 키를 설정한다.
    m_tMapWorkSpace.pMapTex->SetKey(m_tMapWorkSpace.strMapName);


    //필요한 그룹의 수 계산
    CalcGroupNumber(32u * m_tMapWorkSpace.uMapSizeX, 32u * m_tMapWorkSpace.uMapSizeY, 1u);


    //디버그용 쉐이더 바인딩
    //tSBufferDesc SDesc = { eSTRUCT_BUFFER_TYPE::READ_WRITE, eSHADER_PIPELINE_STAGE::__COMPUTE, eCBUFFER_SBUFFER_SHAREDATA_IDX::NONE, idx_t_SRV_NONE, idx_u_SBUFFERRW_DEBUG };
    //m_pSBuffer_Debug = new CStructBuffer(SDesc);

    //int mapsize = m_tMapWorkSpace.uMapSizeX * m_tMapWorkSpace.uMapSizeY;
    //m_DebugData = new tMtrlScalarData[mapsize];
    //size_t bytesize = sizeof(tMtrlScalarData) * mapsize;
    //memset(m_DebugData, 0, bytesize);
    //m_pSBuffer_Debug->Create(sizeof(tMtrlScalarData), mapsize, m_DebugData, mapsize);
    //m_pSBuffer_Debug->BindBufferUAV();

    return true;
}


