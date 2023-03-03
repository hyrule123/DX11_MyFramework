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

CCS_SCMapLoader::CCS_SCMapLoader()
	: CComputeShader(32u, 32u, 1u)	//�ް�Ÿ�� ������ = 32 * 32
    , m_MapSizeX()
    , m_MapSizeY()
    , m_Terrain{}
    //, m_LoadCheck()
    , m_pSBuffer_CV5()
    , m_pSBuffer_VX4()
    , m_pSBuffer_VF4()
    , m_pSBuffer_WPE()
    , m_pSBuffer_VR4()
    , m_pSBuffer_MXTM()
    , m_pSBuffer_Debug()
    , m_DebugData()
{
    //m_LoadRef = 0b00000111;
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

    MapPath += L"Maps/(4) Python 1.4.scx";

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
    bool bLoaded = ReadMapData(szBuffer, FileSize);

    delete[] szBuffer;

    if (true == bLoaded)
        bLoaded = PrepareDataCS();

	return bLoaded;
}

void CCS_SCMapLoader::UnBindCS()
{
    m_pSBuffer_CV5->UnBindSRV();
    m_pTexture->UnBind();



    Debug();
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

bool CCS_SCMapLoader::ReadMapData(char* Data, DWORD Size)
{
    ResetMapData();

    bool bLoaded = false;
    do
    {
        //�о�� �����͸� string�� �ű��.
        std::string_view View(Data, Size);
        string DataStr(View);


        //Terrain Chunk ������(���ڿ�) �� ã�´�.
        size_t pos = 0;
        pos = DataStr.find(TerrainChunk);

        if (pos == string::npos)
            break;

        {
            Chunk* chk = &m_arrMapDataChunk[(int)eSCMAP_DATA_TYPE::TERRAIN];

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
            Chunk* chk = &m_arrMapDataChunk[(int)eSCMAP_DATA_TYPE::MAPSIZE];

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
            Chunk* chk = &m_arrMapDataChunk[(int)eSCMAP_DATA_TYPE::TILEMAP];

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
    {
        ResetMapData();
        return false;
    }

    return true;
}

void CCS_SCMapLoader::ResetMapData()
{
    //m_LoadCheck = 0;

    int Size = (int)eSCMAP_DATA_TYPE::END;

    for (int i = 0; i < Size; ++i)
    {
        SAFE_DELETE_ARRAY(m_arrMapDataChunk[i].Data);
    }

    memset(m_arrMapDataChunk, 0, sizeof(Chunk) * Size);
}

bool CCS_SCMapLoader::PrepareDataCS()
{
    VX4* vx4 = nullptr;
    VR4* vr4 = nullptr;
    WPE* wpe = nullptr;
    VF4* vf4 = nullptr;


    //�� ������ �б�
    if (m_arrMapDataChunk[(int)eSCMAP_DATA_TYPE::MAPSIZE].length != 4)
        return false;

    m_MapSizeX = (int)*(unsigned short*)(m_arrMapDataChunk[(int)eSCMAP_DATA_TYPE::MAPSIZE].Data);
    m_MapSizeY = (int)*(unsigned short*)(m_arrMapDataChunk[(int)eSCMAP_DATA_TYPE::MAPSIZE].Data + 2);

    SetMtrlScalarParam(eMTRLDATA_PARAM_SCALAR::INT_3, m_arrMapDataChunk[(int)eSCMAP_DATA_TYPE::MAPSIZE].Data);


    //MXTM(TILEMAP) ����
    m_pSBuffer_MXTM = new CStructBuffer(eSTRUCT_BUFFER_TYPE::READ_ONLY, eSHADER_PIPELINE_STAGE::__COMPUTE, eCBUFFER_SBUFFER_SHAREDATA_IDX::NONE, e_t_SBUFFER_MXTM, e_u_UAV_NONE);
    UINT DataCount = (UINT)m_arrMapDataChunk[(int)eSCMAP_DATA_TYPE::TILEMAP].length / 16u;

    m_pSBuffer_MXTM->Create(16u, DataCount, m_arrMapDataChunk[(int)eSCMAP_DATA_TYPE::TILEMAP].Data, DataCount);
    m_pSBuffer_MXTM->BindBufferSRV();



    //���� ���� �б�
    if (m_arrMapDataChunk[(int)eSCMAP_DATA_TYPE::TERRAIN].length != 2)
        return false;

    //ù 1����Ʈ�� ���� ������ ����ִ�.
    unsigned char Info = m_arrMapDataChunk[(int)eSCMAP_DATA_TYPE::TERRAIN].Data[0];

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
        return false;
    }

    //CV �ε�
    CV5* cv5 = new CV5[CV5_MAX];
    memset(cv5, 0, sizeof(CV5) * CV5_MAX);
    for (int i = 0; i < CV5_MAX; ++i)
    {
        //��ȸ 20��ŭ �̵��ؼ� ���� �����͸� ������ �ʿ��� 32(sizeof(CV5))��ŭ ������.
        fseek(CV5fp, 20, SEEK_CUR);
        fread(&(cv5[i]), sizeof(CV5), 1, CV5fp);

    }

    if (nullptr == m_pSBuffer_CV5)
    {
        m_pSBuffer_CV5 = new CStructBuffer(eSTRUCT_BUFFER_TYPE::READ_ONLY, eSHADER_PIPELINE_STAGE::__COMPUTE, eCBUFFER_SBUFFER_SHAREDATA_IDX::NONE, e_t_SBUFFER_CV5, e_u_UAV_NONE);
        m_pSBuffer_CV5->Create((UINT)sizeof(CV5), CV5_MAX, cv5, CV5_MAX);
    }
    else
    {
        m_pSBuffer_CV5->UploadData(cv5, CV5_MAX);
    }

    m_pSBuffer_CV5->BindBufferSRV();
    delete[] cv5;


    //VX4
    vx4 = new VX4[VX4_MAX];
    memset(vx4, 0, sizeof(VX4) * VX4_MAX);
    fread(vx4, sizeof(VX4), VX4_MAX, VX4fp);

    if (nullptr == m_pSBuffer_VX4)
    {
        m_pSBuffer_VX4 = new CStructBuffer(eSTRUCT_BUFFER_TYPE::READ_ONLY, eSHADER_PIPELINE_STAGE::__COMPUTE,
            eCBUFFER_SBUFFER_SHAREDATA_IDX::NONE, e_t_SBUFFER_VX4, e_u_UAV_NONE);
        m_pSBuffer_VX4->Create((UINT)sizeof(VX4), VX4_MAX, vx4, VX4_MAX);
    }
    else
    {
        m_pSBuffer_VX4->UploadData(vx4, VX4_MAX);
    }
    m_pSBuffer_VX4->BindBufferSRV();
    delete[] vx4;




    //VR4
    vr4 = new VR4[VR4_MAX];
    memset(vr4, 0, sizeof(VR4) * VR4_MAX);
    fread(vr4, sizeof(VR4), VR4_MAX, VR4fp);

    if (nullptr == m_pSBuffer_VR4)
    {
        m_pSBuffer_VR4 = new CStructBuffer(eSTRUCT_BUFFER_TYPE::READ_ONLY, eSHADER_PIPELINE_STAGE::__COMPUTE,
            eCBUFFER_SBUFFER_SHAREDATA_IDX::NONE, e_t_SBUFFER_VR4, e_u_UAV_NONE);
        m_pSBuffer_VR4->Create((UINT)sizeof(VR4), VR4_MAX, vr4, VR4_MAX);
        m_pSBuffer_VR4->BindBufferSRV();
    }
    else
    {
        m_pSBuffer_VR4->UploadData(vr4, VR4_MAX);
    }
    delete[] vr4;


    //wpe
    wpe = new WPE[WPE_MAX];
    memset(wpe, 0, sizeof(WPE) * WPE_MAX);
    fread(wpe, sizeof(WPE), WPE_MAX, WPEfp);

    size_t size = sizeof(WPE);



    if (nullptr == m_pSBuffer_WPE)
    {
        m_pSBuffer_WPE = new CStructBuffer(eSTRUCT_BUFFER_TYPE::READ_ONLY, eSHADER_PIPELINE_STAGE::__COMPUTE,
            eCBUFFER_SBUFFER_SHAREDATA_IDX::NONE, e_t_SBUFFER_WPE, e_u_UAV_NONE);
        m_pSBuffer_WPE->Create((UINT)sizeof(WPE), WPE_MAX, wpe, WPE_MAX);
        m_pSBuffer_WPE->BindBufferSRV();
    }
    else
    {
        m_pSBuffer_WPE->UploadData(wpe, WPE_MAX);
    }
    delete[] wpe;



    //VF4
    vf4 = new VF4[VF4_MAX];
    memset(vf4, 0, sizeof(VF4) * VF4_MAX);
    fread(vf4, sizeof(VF4), VF4_MAX, VF4fp);

    if (nullptr == m_pSBuffer_VF4)
    {
        m_pSBuffer_VF4 = new CStructBuffer(eSTRUCT_BUFFER_TYPE::READ_ONLY, eSHADER_PIPELINE_STAGE::__COMPUTE,
            eCBUFFER_SBUFFER_SHAREDATA_IDX::NONE, e_t_SBUFFER_VF4, e_u_UAV_NONE);
        m_pSBuffer_VF4->Create((UINT)sizeof(VF4), VF4_MAX, vf4, VF4_MAX);
        m_pSBuffer_VF4->BindBufferSRV();
    }
    else
    {
        m_pSBuffer_VF4->UploadData(vf4, VF4_MAX);
    }

    delete[] vf4;


    //�ؽ�ó
    m_pTexture = new CTexture;
    UINT BindFlag = D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
    m_pTexture->Create(32u * m_MapSizeX, 32u * m_MapSizeY, DXGI_FORMAT_B8G8R8A8_UNORM, BindFlag, D3D11_USAGE::D3D11_USAGE_DEFAULT);
    m_pTexture->BindData_UAV(e_u_TEXTURERW_TARGET);


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


    return true;
}


