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



//"ERA" ���� ���� 
enum class TerrainInfo : unsigned char
{
    Badlands = 0x00,
    SpacePlatform = 0x01,
    Installation = 0x02,
    AshWorld = 0x03,
    Jungle = 0x04,
    Desert = 0x05,
    Ice = 0x06,
    Twilight = 0x07
};

//�� ���� �б��
#define TerrainChunk    "ERA"
#define MapSizeChunk    "DIM"
#define TileMapChunk    "MTXM"

//����ũ��: 52byte, �ް�Ÿ�� �ε���
typedef struct {
    struct CV5Data {
        //�ʿ���� ������
        UINT8 Something[20];
        //�ʿ��� ������. 16���� Mega Tile Index�� ����Ǿ� ����
        UINT16 MegatileIndex[16];
    } group[4096];
}CV5;


//����ũ��: 32byte, ������ɿ���, ����
typedef struct {
    struct VF4Data {
        UINT16 MiniTileFlags[16];
    } data[65536];
}VF4;

//����ũ��: 32bytes, megatile�� � minitile�� �̷�������� ǥ��
typedef struct {
    struct VX4Data {
        UINT16 VR4Index[16];
    } data[65536];
}VX4;

//����ũ��: 64byte - mini tile������ �ȼ��� WPE �ȷ�Ʈ�� �� �� ���� �ش��ϴ����� ����
typedef struct {
    struct VR4Data {
        UINT8 color[64];
    } image[32768];
}VR4;

//����ũ��: 4byte
typedef struct {
    struct WPEData {
        UINT8 r;
        UINT8 g;
        UINT8 b;
        UINT8 padding;
    } data[256];
}WPE;

struct WPEFloatData
{
    float r;
    float g;
    float b;
    float padding;

    WPEFloatData(const WPE::WPEData& Data) :
        r(Data.r / 255.f),
        g(Data.g / 255.f),
        b(Data.b / 255.f),
        padding(1.f)
    {
    }
};


typedef struct RGBAbyte {
    UINT8 r;
    UINT8 g;
    UINT8 b;
    UINT8 a;

    RGBAbyte() :
        r(),
        g(),
        b(),
        a()
    {}
    RGBAbyte(UINT8 _r, UINT8 _g, UINT8 _b, UINT8 _a) :
        r(_r),
        g(_g),
        b(_b),
        a(_a)
    {
    }

}RGBAbyte;

#include <packoff.h>


CCS_SCMapLoader::CCS_SCMapLoader()
	: CComputeShader(32u, 32u, 1u)	//�ް�Ÿ�� ������ = 32 * 32
{
}

CCS_SCMapLoader::~CCS_SCMapLoader()
{
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

	return false;
}

void CCS_SCMapLoader::UnBindCS()
{
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

void CCS_SCMapLoader::LoadTileMap()
{
}


