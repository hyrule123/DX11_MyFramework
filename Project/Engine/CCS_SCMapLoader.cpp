#include "pch.h"
#include "CCS_SCMapLoader.h"


//#ifdef _DEBUG
//#pragma comment (lib, "StormLib_DLL/StormLib_DLL_Debug")
//#else
//#pragma comment (lib, "StormLib_DLL/StormLib_DLL")
//#endif

//#define STORMLIB_NO_AUTO_LINK
//#include <StormLib_DLL/StormLib.h>


//�ϴ� ���⼭ ���Ƽ� ����ų����
//���߿� ����� �л��ų��
#include "CPathMgr.h"
#include "CStructBuffer.h"



#ifdef _DEBUG
constexpr const wchar_t* strStormLibPath = L"StormLib_DLL_Debug.dll";
#else
constexpr const wchar_t* strStormLibPath = L"StormLib_DLL.dll";
#endif

//�� ���� �б��
constexpr const char* TerrainChunk = "ERA";
constexpr const char* MapSizeChunk = "DIM";
constexpr const char* TileMapChunk = "MTXM";

CCS_SCMapLoader::CCS_SCMapLoader()
	: CComputeShader(32u, 32u, 1u)	//�ް�Ÿ�� ������ = 32 * 32
    , m_arrpSBufferTileSet{}
    , m_pSBuffer_MXTM()
    , m_pSBuffer_Debug()
    , m_DebugData()
    
{

    wstring Path = CPathMgr::GetInst()->GetContentPath();
    Path += L"Maps/Tilesets/";


    //Ÿ�ϼ� �����͸� ������ �޸𸮰��� �����Ҵ� 
    tTileSet* Tileset = new tTileSet;
    for (UINT8 TileSetIdx = (UINT8)0u; TileSetIdx < (UINT8)eTILESET_INFO::END; ++TileSetIdx)
    {
        //Ÿ�ϼ� ������ �ʱ�ȭ
        memset(Tileset, 0, sizeof(tTileSet));
        wstring FullPath = Path;

        switch ((eTILESET_INFO)TileSetIdx)
        {
        case eTILESET_INFO::BADLANDS:
            FullPath += TEXT("badlands");
            break;
        case eTILESET_INFO::SPACE_PLATFORM:
            FullPath += TEXT("platform");
            break;
        case eTILESET_INFO::INSTALLATION:
            FullPath += TEXT("install");
            break;
        case eTILESET_INFO::ASH_WORLD:
            FullPath += TEXT("ashworld");
            break;
        case eTILESET_INFO::JUNGLE:
            FullPath += TEXT("jungle");
            break;
        case eTILESET_INFO::DESERT:
            FullPath += TEXT("Desert");
            break;
        case eTILESET_INFO::ICE:
            FullPath += TEXT("Ice");
            break;
        case eTILESET_INFO::TWILIGHT:
            FullPath += TEXT("Twilight");
            break;
        default:
            break;
        }

        FILE* fpCV5, * fpVX4, * fpVR4, * fpWPE, * fpVF4;
        _wfopen_s(&fpCV5, (FullPath + TEXT(".CV5")).c_str(), TEXT("rb"));
        _wfopen_s(&fpVX4, (FullPath + TEXT(".VX4")).c_str(), TEXT("rb"));
        _wfopen_s(&fpVR4, (FullPath + TEXT(".VR4")).c_str(), TEXT("rb"));
        _wfopen_s(&fpWPE, (FullPath + TEXT(".WPE")).c_str(), TEXT("rb"));
        _wfopen_s(&fpVF4, (FullPath + TEXT(".VF4")).c_str(), TEXT("rb"));


        if (fpCV5 == nullptr || fpVX4 == nullptr || fpVR4 == nullptr || fpWPE == nullptr || fpVF4 == nullptr)
        {
            _fcloseall();
            delete Tileset;
            MessageBoxA(nullptr, "Tileset Load Failed", nullptr, MB_OK);
            assert(nullptr);
        }



        for (int i = 0; i < CV5_MAX; ++i)
        {
            //��ȸ 20��ŭ �̵��ؼ� ���� �����͸� ������ �ʿ��� 32(sizeof(CV5))��ŭ ������.
            fseek(fpCV5, 20, SEEK_CUR);
            size_t BytesRead = fread(&(Tileset->cv5[i]), sizeof(CV5), 1, fpCV5);

            if ((size_t)0u == BytesRead)
                break;
        }

        fread(Tileset->vx4, sizeof(VX4), VX4_MAX, fpVX4);
        fread(Tileset->vr4, sizeof(VR4), VR4_MAX, fpVR4);
        fread(Tileset->wpe, sizeof(WPE), WPE_MAX, fpWPE);
        fread(Tileset->vf4, sizeof(VF4), VF4_MAX, fpVF4);

        //Desc �ۼ��ؼ� SBuffer ����
        tSBufferDesc SDesc = { eSTRUCT_BUFFER_TYPE::READ_ONLY,
        eSHADER_PIPELINE_STAGE::__COMPUTE,
        eCBUFFER_SBUFFER_SHAREDATA_IDX::NONE,
        e_t_SRV_NONE,
        e_u_UAV_NONE
        };
        //Ÿ�ϼ� �ϰ������� ���� �Ҵ�
        

        m_arrpSBufferTileSet[TileSetIdx].arrTileSetMember = new CStructBuffer[(int)eTILESET_MEMBER::END];
        for (int i = 0; i < (int)eTILESET_MEMBER::END; ++i)
        {
            //0 ~ 5������ ��ġ���� ������.
            SDesc.i_e_t_SRVIdx = i;

            //Desc ����
            m_arrpSBufferTileSet[TileSetIdx].arrTileSetMember[i].SetDesc(SDesc);

            //���ڿ��� �´� ����ȭ���� ���� ����
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
    wstring MapPath = CPathMgr::GetInst()->GetContentPath();

    MapPath += L"Maps/";
    MapPath += m_tMapWorkSpace.wstrMapName;

    HANDLE hMpq = NULL;          // Open archive handle
    HANDLE hFile = NULL;          // Archived file handle
    //HANDLE handle = NULL;          // Disk file handle

    //DLL �ε�
    HMODULE DLLModule = LoadLibraryW(strStormLibPath);
    if (nullptr == DLLModule)
        return false;

    //�Լ� ���
    //MPQ ���� �ε�
    typedef bool(__stdcall* pSFileOpenArchive)(const TCHAR*, DWORD, DWORD, HANDLE*);
    pSFileOpenArchive MpqOpenFunc = (pSFileOpenArchive)GetProcAddress(DLLModule, "SFileOpenArchive");
    
    typedef bool(__stdcall* pSFileOpenFileEx)(HANDLE, const char*, DWORD, HANDLE*);
    pSFileOpenFileEx funcOpenScenarioChk = (pSFileOpenFileEx)GetProcAddress(DLLModule, "SFileOpenFileEx");

    //DWORD WINAPI SFileGetFileSize(HANDLE hFile, LPDWORD pdwFileSizeHigh);
    typedef DWORD(__stdcall* pSFileGetFileSize)(HANDLE, LPDWORD);
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


    //DLL�κ��� �ε� ���� �Ǵ� �Լ����� �ε带 �������� ��� false ��ȯ
    if (nullptr == MpqOpenFunc || false == MpqOpenFunc(MapPath.c_str(), 0, 0, &hMpq))
    {
        int nError = GetLastError();
        return false;
    }
        
    //MPQ ���� ������ scenario.chk ���� Ž��.
    if (nullptr == funcOpenScenarioChk || false == funcOpenScenarioChk(hMpq, "staredit\\scenario.chk", 0, &hFile))
    {
        int nError = GetLastError();
        return false;
    }
        

    // Read the file from the archive
    char* szBuffer;
    DWORD dwBytes = 1;

    //������ ����� �޾ƿ� �� �ش� ������ �޾ƿ� ������ �����Ҵ� �� �ʱ�ȭ�Ѵ�.
    DWORD FileSize = funcGetFileSize(hFile, NULL);
    szBuffer = new char[FileSize];
    memset(szBuffer, 0, (size_t)FileSize);


    //�����͸� �о�´�.
    funcReadFile(hFile, szBuffer, FileSize, &dwBytes, NULL);

    
    //���� ���� ����
    funcCloseFile(hFile);
    funcCloseArchive(hMpq);
    
    //���̺귯�� ���� ����
    FreeLibrary(DLLModule);


    //szbuffer�� ���� �����Ͱ� �������� ����.
    //���⼭ ���� �׷����µ� �ʿ��� ������ �����;� ��
    bool bLoaded = ReadMapData(szBuffer, FileSize);

    delete[] szBuffer;

    //�ʵ����͸� �о���°ͱ��� �������� ��� CS�� �����͸� ���ε��Ѵ�.
    if (true == bLoaded)
        bLoaded = UploadMapDataToCS();

	return bLoaded;



    //�Ͻ��� ��ŷ �ڵ�
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
//        //�� ������ scenario.chk �����͸� ã�´�.
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
//    //ã�� scenario.chk ���Ϸκ��� ������ �о�´�.
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
    for (int i = 0; i < (int)eTILESET_MEMBER::END; ++i)
    {
        m_arrpSBufferTileSet[(int)m_tMapWorkSpace.eTileSet].arrTileSetMember[i].UnBindSRV();
    }
    
    
    //�� ������ ����
    SAFE_DELETE(m_pSBuffer_MXTM);

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

bool CCS_SCMapLoader::LoadMap(const wstring& _wstrMapName, __out tMapData& _tMapData)
{
    _tMapData = tMapData();

    m_tMapWorkSpace.wstrMapName = _wstrMapName;

    //�ε� ���� �� ���� ���۷����� �ʱ�ȭ�ϰ� false ��ȯ
    if (false == Execute())
        return false;
            
    //���� �� �۾� ������ �Ѱ��ְ� �ڽ��� �۾� ������ �ʱ�ȭ�Ѵ�.
    _tMapData = m_tMapWorkSpace;
    m_tMapWorkSpace = tMapData();
    return true;
}


bool CCS_SCMapLoader::ReadMapData(char* Data, DWORD Size)
{
    //tMapDataChunk ��ü�� ������ ������� ũ�� �����Ƿ� ���ÿ� ����
    tMapDataChunk arrMapDataChunk[(int)eSCMAP_DATA_TYPE::END] = {};

    bool bLoaded = false;
    do
    {
        //�о�� �����͸� string�� �ű��.
        std::string_view View(Data, Size);
        string DataStr(View);


        //Terrain tMapDataChunk ������(���ڿ�) �� ã�´�.
        size_t pos = 0;
        pos = DataStr.find(TerrainChunk);

        if (pos == string::npos)
            break;

        {
            tMapDataChunk* chk = &arrMapDataChunk[(int)eSCMAP_DATA_TYPE::TERRAIN];

            char* adress = Data + pos;

            //�̸� ����
            memcpy(chk->TypeName, adress, 4);

            //������ ��� ������ ����
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

            //�̸� ����
            memcpy(chk->TypeName, adress, 4);

            //������ ����� ������ ����
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
            tMapDataChunk* chk = &arrMapDataChunk[(int)eSCMAP_DATA_TYPE::TILEMAP];

            char* adress = Data + pos;

            //�̸� ����
            memcpy(chk->TypeName, adress, 4);

            //������ ����� ������ ����
            int* len = (int*)(adress + 4);
            chk->length = *len;

            chk->Data = new unsigned char[chk->length + 1];
            memcpy(chk->Data, adress + 8, chk->length);
        }

        bLoaded = true;

    } while (false);

    if (false == bLoaded)
        return false;


    //�� ������ �б�
    if (arrMapDataChunk[(int)eSCMAP_DATA_TYPE::MAPSIZE].length != 4)
        return false;

    m_tMapWorkSpace.uMapSizeX = (int)*(unsigned short*)(arrMapDataChunk[(int)eSCMAP_DATA_TYPE::MAPSIZE].Data);
    m_tMapWorkSpace.uMapSizeY = (int)*(unsigned short*)(arrMapDataChunk[(int)eSCMAP_DATA_TYPE::MAPSIZE].Data + 2);

    Vec2 vMapSize = Vec2(m_tMapWorkSpace.uMapSizeX, m_tMapWorkSpace.uMapSizeY);
    SetMtrlScalarParam(MTRL_SCALAR_VEC2_MAPSIZE, &vMapSize);

    //MXTM(TILEMAP) ���� �� ����
    SAFE_DELETE(m_pSBuffer_MXTM);
    tSBufferDesc SBufferDesc = { eSTRUCT_BUFFER_TYPE::READ_ONLY, eSHADER_PIPELINE_STAGE::__COMPUTE, eCBUFFER_SBUFFER_SHAREDATA_IDX::NONE, e_t_SBUFFER_MXTM, e_u_UAV_NONE };
   
    m_pSBuffer_MXTM = new CStructBuffer(SBufferDesc);
    UINT DataCount = (UINT)arrMapDataChunk[(int)eSCMAP_DATA_TYPE::TILEMAP].length / 16u;
    m_pSBuffer_MXTM->Create((UINT)sizeof(MXTM), DataCount, arrMapDataChunk[(int)eSCMAP_DATA_TYPE::TILEMAP].Data, DataCount);
    m_pSBuffer_MXTM->BindBufferSRV();


    //���� ���� �б�
    if (arrMapDataChunk[(int)eSCMAP_DATA_TYPE::TERRAIN].length != 2)
        return false;

    //ù 1����Ʈ�� ���� ������ ����ִ�.
    unsigned char Info = arrMapDataChunk[(int)eSCMAP_DATA_TYPE::TERRAIN].Data[0];

    //���� 4��Ʈ�� 0���� ��ȯ
    unsigned char bitshift = 0b00001111;
    Info &= bitshift;

    //Ȥ�ó� ��Ʈ����Ʈ ���� 8�� �ʰ��ϴ� ���� ���� ��� 8�� ���ش�.
    if (Info >= (unsigned char)8)
        Info -= (unsigned char)8;

    m_tMapWorkSpace.eTileSet = static_cast<eTILESET_INFO>(Info);

    return true;
}

bool CCS_SCMapLoader::UploadMapDataToCS()
{
    //Ÿ�ϼ��� ���ε����ش�.
    for (int i = 0; i < (int)eTILESET_MEMBER::END; ++i)
    {
        m_arrpSBufferTileSet[(int)m_tMapWorkSpace.eTileSet].arrTileSetMember[i].BindBufferSRV();
    }
    

    //Ÿ���� �� �ؽ�ó�� �����Ҵ��ϰ�, UAV�� ���ε�
    m_tMapWorkSpace.pMapTex = new CTexture;
    UINT BindFlag = D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
    m_tMapWorkSpace.pMapTex->Create(32u * m_tMapWorkSpace.uMapSizeX, 32u * m_tMapWorkSpace.uMapSizeY, DXGI_FORMAT_B8G8R8A8_UNORM, BindFlag, D3D11_USAGE::D3D11_USAGE_DEFAULT);
    m_tMapWorkSpace.pMapTex->BindData_UAV(e_u_TEXTURERW_TARGET);


    //�ʿ��� �׷��� �� ���
    CalcGroupNumber(32u * m_tMapWorkSpace.uMapSizeX, 32u * m_tMapWorkSpace.uMapSizeY, 1u);


    //����׿� ���̴� ���ε�
    tSBufferDesc SDesc = { eSTRUCT_BUFFER_TYPE::READ_WRITE, eSHADER_PIPELINE_STAGE::__COMPUTE, eCBUFFER_SBUFFER_SHAREDATA_IDX::NONE, e_t_SRV_NONE, e_u_SBUFFERRW_DEBUG };
    m_pSBuffer_Debug = new CStructBuffer(SDesc);

    int mapsize = m_tMapWorkSpace.uMapSizeX * m_tMapWorkSpace.uMapSizeY;
    m_DebugData = new tMtrlScalarData[mapsize];
    size_t bytesize = sizeof(tMtrlScalarData) * mapsize;
    memset(m_DebugData, 0, bytesize);
    m_pSBuffer_Debug->Create(sizeof(tMtrlScalarData), mapsize, m_DebugData, mapsize);
    m_pSBuffer_Debug->BindBufferUAV();

    return true;
}


