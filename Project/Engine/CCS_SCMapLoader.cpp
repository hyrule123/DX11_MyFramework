#include "pch.h"
#include "CCS_SCMapLoader.h"


#ifdef _DEBUG
#pragma comment (lib, "StormLib_DLL/StormLib_DLL_Debug")
#else
#pragma comment (lib, "StormLib_DLL/StormLib_DLL")
#endif

#define STORMLIB_NO_AUTO_LINK
#include <StormLib_DLL/StormLib.h>


//�ϴ� ���⼭ ���Ƽ� ����ų����
//���߿� ����� �л��ų��
#include "CPathMgr.h"
#include "CStructBuffer.h"


#include "Shader_SCMapLoader_0_Header.hlsli"


//�� ���� �б��
#define TerrainChunk    "ERA"
#define MapSizeChunk    "DIM"
#define TileMapChunk    "MTXM"

//����ũ��: 52byte, �ް�Ÿ�� �ε���




////����ũ��: 32byte, ������ɿ���, ����
//typedef struct {
//    struct VF4Data {
//        UINT16 MiniTileFlags[16];
//    } data[65536];
//}VF4;
//
////����ũ��: 32bytes, megatile�� � minitile�� �̷�������� ǥ��
//typedef struct {
//    struct VX4Data {
//        UINT16 VR4Index[16];
//    } data[65536];
//}VX4;
//
////����ũ��: 64byte - mini tile������ �ȼ��� WPE �ȷ�Ʈ�� �� �� ���� �ش��ϴ����� ����
//typedef struct {
//    struct VR4Data {
//        UINT8 color[64];
//    } image[32768];
//}VR4;
//
////����ũ��: 4byte
//typedef struct {
//    struct WPEData {
//        UINT8 r;
//        UINT8 g;
//        UINT8 b;
//        UINT8 padding;
//    } data[256];
//}WPE;
//
//struct WPEFloatData
//{
//    float r;
//    float g;
//    float b;
//    float padding;
//
//    WPEFloatData(const WPE::WPEData& Data) :
//        r(Data.r / 255.f),
//        g(Data.g / 255.f),
//        b(Data.b / 255.f),
//        padding(1.f)
//    {
//    }
//};
//
//
//typedef struct RGBAbyte {
//    UINT8 r;
//    UINT8 g;
//    UINT8 b;
//    UINT8 a;
//
//    RGBAbyte() :
//        r(),
//        g(),
//        b(),
//        a()
//    {}
//    RGBAbyte(UINT8 _r, UINT8 _g, UINT8 _b, UINT8 _a) :
//        r(_r),
//        g(_g),
//        b(_b),
//        a(_a)
//    {
//    }
//
//}RGBAbyte;
//
//#include <packoff.h>


CCS_SCMapLoader::CCS_SCMapLoader()
	: CComputeShader(32u, 32u, 1u)	//�ް�Ÿ�� ������ = 32 * 32
    , m_MapSizeX()
    , m_MapSizeY()
    , m_Terrain{}
    , m_LoadCheck()
    , m_pSBuffer_CV5()
    , m_pSBuffer_VX4()
    , m_pSBuffer_VF4()
    , m_pSBuffer_WPE()
    , m_pSBuffer_VR4()
    , m_pSBuffer_MXTM()
    , m_pSBuffer_Debug()
    , m_DebugData()
{
    m_LoadRef = 0b00000111;
}

CCS_SCMapLoader::~CCS_SCMapLoader()
{
    DESTRUCTOR_DELETE(m_pSBuffer_MXTM);
    DESTRUCTOR_DELETE(m_pSBuffer_CV5);
    DESTRUCTOR_DELETE(m_pSBuffer_VX4);
    DESTRUCTOR_DELETE(m_pSBuffer_VF4);
    DESTRUCTOR_DELETE(m_pSBuffer_VR4);
    DESTRUCTOR_DELETE(m_pSBuffer_WPE);
    DESTRUCTOR_DELETE(m_pSBuffer_Debug);

    if (nullptr != m_DebugData)
        delete[] m_DebugData;
    
}

bool CCS_SCMapLoader::BindDataCS()
{
    wstring MapPath = CPathMgr::GetInst()->GetContentPath();

    MapPath += L"Maps/(4)Fighting Sprit 1.3.scx";

    HANDLE hMpq = NULL;          // Open archive handle
    HANDLE hFile = NULL;          // Archived file handle
    //HANDLE handle = NULL;          // Disk file handle
    int    nError = ERROR_SUCCESS; // Result value


    // Open an archive, e.g. "d2music.mpq"
    if (nError == ERROR_SUCCESS)
    {
        if (!SFileOpenArchive(MapPath.c_str(), 0, 0, &hMpq))
            nError = GetLastError();
    }


    // Open a file in the archive, e.g. "data\global\music\Act1\tristram.wav"
    if (nError == ERROR_SUCCESS)
    {
        //�� ������ scenario.chk �����͸� ã�´�.
        if (!SFileOpenFileEx(hMpq, "staredit\\scenario.chk", 0, &hFile))
            nError = GetLastError();
    }

    if (nError != ERROR_SUCCESS)
        return false;

    // Read the file from the archive
    char* szBuffer;
    DWORD dwBytes = 1;

    //ã�� scenario.chk ���Ϸκ��� ������ �о�´�.
    DWORD FileSize = SFileGetFileSize(hFile, NULL);

    szBuffer = new char[FileSize];

    SFileReadFile(hFile, szBuffer, FileSize, &dwBytes, NULL);

    // Cleanup and exit
//if (handle != NULL)
//    CloseHandle(handle);
    if (hFile != NULL)
        SFileCloseFile(hFile);
    if (hMpq != NULL)
        SFileCloseArchive(hMpq);

    //szbuffer�� ���� �����Ͱ� �������� ����.
    //���⼭ ���� �׷����µ� �ʿ��� ������ �����;� ��
    ReadMapData(szBuffer, FileSize);

    delete[] szBuffer;

	return true;
}

void CCS_SCMapLoader::UnBindCS()
{
    m_pSBuffer_CV5->UnBindSRV();
    m_pTexture->UnBind();
}

void CCS_SCMapLoader::Debug()
{
    m_pSBuffer_Debug->GetData(m_DebugData, sizeof(tMtrlScalarData) * m_MapSizeX * m_MapSizeY);

    for (int i = 0; i < m_MapSizeX * m_MapSizeY; ++i)
    {
        tMtrlScalarData Temp = m_DebugData[i];

        int a = 0;
    }

    
}

void CCS_SCMapLoader::ReadMapData(char* Data, DWORD Size)
{
    ResetMapData();

    //�о�� �����͸� string�� �ű��.
    string DataStr(Data, Data + Size);

    //Terrain Chunk ������(���ڿ�) �� ã�´�.
    size_t pos = 0;
    pos = DataStr.find(TerrainChunk);

    if (pos != string::npos)
    {
        Chunk* chk = &m_MapDataChunk[(int)eSCMAP_DATA_TYPE::TERRAIN];

        char* adress = Data + pos;

        //�̸� ����
        memcpy(chk->TypeName, adress, 4);

        //������ ��� ������ ����
        int* len = (int*)(adress + 4);
        chk->length = *len;

        chk->Data = new unsigned char[chk->length + 1];
        memcpy(chk->Data, adress + 8, chk->length);

        m_LoadCheck |= 1 << (int)eSCMAP_DATA_TYPE::TERRAIN;
    }

    pos = 0;
    pos = DataStr.find(MapSizeChunk);

    if (pos != std::string::npos)
    {
        Chunk* chk = &m_MapDataChunk[(int)eSCMAP_DATA_TYPE::MAPSIZE];

        char* adress = Data + pos;

        //�̸� ����
        memcpy(chk->TypeName, adress, 4);

        //������ ����� ������ ����
        int* len = (int*)(adress + 4);
        chk->length = *len;

        chk->Data = new unsigned char[chk->length + 1];
        memcpy(chk->Data, adress + 8, chk->length);

        m_LoadCheck |= 1 << (int)eSCMAP_DATA_TYPE::MAPSIZE;
    }


    pos = 0;
    pos = DataStr.find(TileMapChunk);

    if (pos != std::string::npos)
    {
        Chunk* chk = &m_MapDataChunk[(int)eSCMAP_DATA_TYPE::TILEMAP];

        char* adress = Data + pos;

        //�̸� ����
        memcpy(chk->TypeName, adress, 4);

        //������ ����� ������ ����
        int* len = (int*)(adress + 4);
        chk->length = *len;

        chk->Data = new unsigned char[chk->length + 1];
        memcpy(chk->Data, adress + 8, chk->length);

        m_LoadCheck |= 1 << (int)eSCMAP_DATA_TYPE::TILEMAP;
    }


    if (m_LoadCheck != m_LoadRef)
        ResetMapData();


    LoadTileMap();
}

void CCS_SCMapLoader::ResetMapData()
{
    m_LoadCheck = 0;

    int Size = (int)eSCMAP_DATA_TYPE::END;

    for (int i = 0; i < Size; ++i)
    {
        if(nullptr != m_MapDataChunk)
            delete[] m_MapDataChunk[i].Data;
    }

    memset(m_MapDataChunk, 0, sizeof(Chunk) * Size);
}

bool CCS_SCMapLoader::LoadTileMap()
{
    if (false == LoadComplete())
        return false;


    VX4* vx4;
    VR4* vr4;
    WPE* wpe;
    VF4* vf4;


    //�� ������ �б�
    if (m_MapDataChunk[(int)eSCMAP_DATA_TYPE::MAPSIZE].length != 4)
        return false;

    int w = (int)*(unsigned short*)(m_MapDataChunk[(int)eSCMAP_DATA_TYPE::MAPSIZE].Data);
    int h = (int)*(unsigned short*)(m_MapDataChunk[(int)eSCMAP_DATA_TYPE::MAPSIZE].Data + 2);
    m_MapSizeX = w;
    m_MapSizeY = h;



    //MXTM(TILEMAP) ����
    m_pSBuffer_MXTM = new CStructBuffer(eSTRUCT_BUFFER_TYPE::READ_ONLY, eSHADER_PIPELINE_STAGE::__COMPUTE, eCBUFFER_SBUFFER_SHAREDATA_IDX::NONE, e_t_SBUFFER_MXTM, e_u_UAV_NONE);
    UINT DataCount = (UINT)m_MapDataChunk[(int)eSCMAP_DATA_TYPE::TILEMAP].length / 16u;

    m_pSBuffer_MXTM->Create(16u, DataCount, m_MapDataChunk[(int)eSCMAP_DATA_TYPE::TILEMAP].Data, DataCount);
    m_pSBuffer_MXTM->BindBufferSRV();

    //FILE* MXTMData;
    //_wfopen_s(&MXTMData, L"D:/Users/ekdrn/Desktop/TESTMXTM", L"wb");

    //fwrite(m_MapDataChunk[(int)eSCMAP_DATA_TYPE::TILEMAP].Data, m_MapDataChunk[(int)eSCMAP_DATA_TYPE::TILEMAP].length, 1, MXTMData);

    //fclose(MXTMData);

    int x, y, subx, suby, i, j;





    //���� ���� �б�
    if (m_MapDataChunk[(int)eSCMAP_DATA_TYPE::TERRAIN].length != 2)
        return false;

    //ù 1����Ʈ�� ���� ������ ����ִ�.
    unsigned char Info = m_MapDataChunk[(int)eSCMAP_DATA_TYPE::TERRAIN].Data[0];

    //���� 4��Ʈ�� 0���� ��ȯ
    unsigned char bitshift = 0b11110000;
    Info &= ~bitshift;

    //Ȥ�ó� ��Ʈ����Ʈ ���� 8�� �ʰ��ϴ� ���� ���� ��� 8�� ���ش�.
    if (Info >= (unsigned char)8)
        Info -= (unsigned char)8;

    m_Terrain = static_cast<TerrainInfo>(Info);

    wstring Path = CPathMgr::GetInst()->GetContentPath();
    wstring FullPath = Path + L"Maps/Tilesets/";

    switch ((TerrainInfo)Info)
    {
    case TerrainInfo::Badlands:
        FullPath += TEXT("badlands");
        break;
    case TerrainInfo::SpacePlatform:
        FullPath += TEXT("platform");
        break;
    case TerrainInfo::Installation:
        FullPath += TEXT("install");
        break;
    case TerrainInfo::AshWorld:
        FullPath += TEXT("ashworld");
        break;
    case TerrainInfo::Jungle:
        FullPath += TEXT("jungle");
        break;
    case TerrainInfo::Desert:
        FullPath += TEXT("Desert");
        break;
    case TerrainInfo::Ice:
        FullPath += TEXT("Ice");
        break;
    case TerrainInfo::Twilight:
        FullPath += TEXT("Twilight");
        break;
    default:
        break;
    }

    FILE* CV5fp, * VX4fp, * VR4fp, * WPEfp, * VF4fp;
    _wfopen_s(&CV5fp, (FullPath + TEXT(".CV5")).c_str(), TEXT("rb"));
    _wfopen_s(&VX4fp, (FullPath + TEXT(".VX4")).c_str(), TEXT("rb"));
    _wfopen_s(&VR4fp, (FullPath + TEXT(".VR4")).c_str(), TEXT("rb"));
    _wfopen_s(&WPEfp, (FullPath + TEXT(".WPE")).c_str(), TEXT("rb"));
    _wfopen_s(&VF4fp, (FullPath + TEXT(".VF4")).c_str(), TEXT("rb"));


    if (CV5fp == NULL || VX4fp == NULL || VR4fp == NULL || WPEfp == NULL) {
        _fcloseall();
        return 0;
    }

    //CV �ε�
    CV5* cv5 = new CV5[4096];
    memset(cv5, 0, sizeof(CV5) * 4096);
    for (int i = 0; i < 4096; ++i)
    {
        //��ȸ 20��ŭ �̵��ؼ� ���� �����͸� ������ �ʿ��� 32(sizeof(CV5))��ŭ ������.
        fseek(CV5fp, 20, SEEK_CUR);
        fread(&(cv5[i]), sizeof(CV5), 1, CV5fp);
    }
    m_pSBuffer_CV5 = new CStructBuffer(eSTRUCT_BUFFER_TYPE::READ_ONLY, eSHADER_PIPELINE_STAGE::__COMPUTE, eCBUFFER_SBUFFER_SHAREDATA_IDX::NONE, e_t_SBUFFER_CV5, e_u_UAV_NONE);
    m_pSBuffer_CV5->Create((UINT)sizeof(CV5), 4096u, cv5, 4096u);
    m_pSBuffer_CV5->BindBufferSRV();
    delete[] cv5;


    //VX4
    vx4 = new VX4[65536];
    memset(vx4, 0, sizeof(VX4) * 65536);
    fread(vx4, sizeof(VX4), 65536, VX4fp);

    m_pSBuffer_VX4 = new CStructBuffer(eSTRUCT_BUFFER_TYPE::READ_ONLY, eSHADER_PIPELINE_STAGE::__COMPUTE,
        eCBUFFER_SBUFFER_SHAREDATA_IDX::NONE, e_t_SBUFFER_VX4, e_u_UAV_NONE);

    m_pSBuffer_VX4->Create((UINT)sizeof(VX4), 65536, vx4, 65536);
    m_pSBuffer_VX4->BindBufferSRV();
    delete[] vx4;




    //VR4
    vr4 = new VR4[32768];
    memset(vr4, 0, sizeof(VR4) * 32768);
    fread(vr4, sizeof(VR4), 32768, VR4fp);

    m_pSBuffer_VR4 = new CStructBuffer(eSTRUCT_BUFFER_TYPE::READ_ONLY, eSHADER_PIPELINE_STAGE::__COMPUTE,
        eCBUFFER_SBUFFER_SHAREDATA_IDX::NONE, e_t_SBUFFER_VR4, e_u_UAV_NONE);

    m_pSBuffer_VR4->Create((UINT)sizeof(VR4), 32768, vr4, 32768);
    m_pSBuffer_VR4->BindBufferSRV();
    delete[] vr4;


    //wpe
    wpe = new WPE[64];
    memset(wpe, 0, sizeof(WPE) * 64);
    fread(wpe, sizeof(WPE), 64, WPEfp);

    m_pSBuffer_WPE = new CStructBuffer(eSTRUCT_BUFFER_TYPE::READ_ONLY, eSHADER_PIPELINE_STAGE::__COMPUTE,
        eCBUFFER_SBUFFER_SHAREDATA_IDX::NONE, e_t_SBUFFER_WPE, e_u_UAV_NONE);

    m_pSBuffer_WPE->Create((UINT)sizeof(WPE), 64, wpe, 64);
    m_pSBuffer_WPE->BindBufferSRV();
    delete[] wpe;



    //VF4
    vf4 = new VF4[65536];
    memset(vf4, 0, sizeof(WPE) * 65536);
    fread(vf4, sizeof(WPE), 65536, VF4fp);

    m_pSBuffer_VF4 = new CStructBuffer(eSTRUCT_BUFFER_TYPE::READ_ONLY, eSHADER_PIPELINE_STAGE::__COMPUTE,
        eCBUFFER_SBUFFER_SHAREDATA_IDX::NONE, e_t_SBUFFER_VF4, e_u_UAV_NONE);

    m_pSBuffer_VF4->Create((UINT)sizeof(WPE), 65536, vf4, 65536);
    m_pSBuffer_VF4->BindBufferSRV();
    delete[] vf4;


   

    m_pTexture = new CTexture;
    UINT BindFlag = D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
    m_pTexture->Create(32u * m_MapSizeX, 32u * m_MapSizeY, DXGI_FORMAT_B8G8R8A8_UNORM, BindFlag, D3D11_USAGE::D3D11_USAGE_DEFAULT);
    m_pTexture->BindData_UAV(e_t_TEXTURE_TARGET);


    CalcGroupNumber(32u * m_MapSizeX, 32u * m_MapSizeY, 1u);

    _fcloseall();


    //����׿� ���̴� ���ε�
    m_pSBuffer_Debug = new CStructBuffer(eSTRUCT_BUFFER_TYPE::READ_WRITE, eSHADER_PIPELINE_STAGE::__COMPUTE, eCBUFFER_SBUFFER_SHAREDATA_IDX::NONE, e_t_SRV_NONE, e_u_SBUFFERRW_DEBUG);

    int mapsize = m_MapSizeX * m_MapSizeY;
    m_DebugData = new tMtrlScalarData[mapsize];
    size_t bytesize = sizeof(tMtrlScalarData) * mapsize;
    memset(m_DebugData, 0, bytesize);
    m_pSBuffer_Debug->Create(sizeof(tMtrlScalarData), mapsize, m_DebugData, mapsize);
    m_pSBuffer_Debug->BindBufferUAV();


    UINT GroupAndIndex = 0u;

    UINT uGroupID_Xdiv8 = 0 / MXTM_PACK;
    UINT MapSizeDiv8 = 16u;

    UINT IdxInPack = 0 % MXTM_PACK;

    UINT32 res = 0u;

    //uint�� �ε��� ��ȣ -> ��Ʈ����Ʈ 1���ָ� 2�� ���� ȿ��
    UINT32 Idx = IdxInPack >> 1u;

    //�պκ����� �޺κ����� : 2�� ���� ������ ( & ������ )
    UINT32 FrontOrBack = (IdxInPack & 0x00000001) * 16u;

    MXTM st = {};

    memcpy(&st, m_MapDataChunk[(int)eSCMAP_DATA_TYPE::TILEMAP].Data, 16u);



    //FILE* testfilechunk;
    //_wfopen_s(&testfilechunk, L"D:/Users/ekdrn/Desktop/TestFileChunk", L"wb");
    //fwrite(&st, 16, 1, testfilechunk);

    //fclose(testfilechunk);


    res = ((0xFFFF0000 >> FrontOrBack) & st.MXTM_Pack.u32[Idx]) >> (16u - FrontOrBack);

    UINT16 test = (0x0000ffff & st.MXTM_Pack.u32[1]);



    int group = test >> 4;
    int index = test & 0xf;

    int stop = 0;

    UINT16 dest = {};
    memcpy(&dest, m_MapDataChunk[(int)eSCMAP_DATA_TYPE::TILEMAP].Data, 2u);
    group = res >> 4;
    index = res & 0xf;

    stop = 0;

    //GroupAndIndex = ExtractUINT16FromUINT32_4(g_SBuffer_MXTM[_uGroupID_XY.y * MapSizeDiv8.x + uGroupID_Xdiv8].MXTM_Pack, IdxInPack);


    return true;
}


