#pragma once
#include "CComputeShader.h"

#include "Shader_SCMapLoader_0_Header.hlsli"

#include "ptr.h"
#include "CTexture.h"

//"ERA" 안의 정보 
enum class eTERRAIN_INFO : UINT8
{
    BADLANDS = 0x00,
    SPACE_PLATFORM = 0x01,
    INSTALLATION = 0x02,
    ASH_WORLD = 0x03,
    JUNGLE = 0x04,
    DESERT = 0x05,
    ICE = 0x06,
    TWILIGHT = 0x07,
    END = 0x08
};


//struct tpSBufferTileSet
//{
//    CStructBuffer* pSBuffer_CV5;
//    CStructBuffer* pSBuffer_VX4;
//    CStructBuffer* pSBuffer_VF4;
//    CStructBuffer* pSBuffer_VR4;
//    CStructBuffer* pSBuffer_WPE;
//};


//맵데이터로부터 로드해야하는 데이터들의 플래그
enum class eSCMAP_DATA_TYPE
{
    TERRAIN,
    MAPSIZE,
    TILEMAP,
    END
};

typedef struct Chunk
{
    char TypeName[5];
    unsigned long length;
    unsigned char* Data;

    Chunk() :
        TypeName{},
        length(), Data()
    {
    }
    ~Chunk()
    {
        SAFE_DELETE_ARRAY(Data);
    }


} Chunk;


struct tMapData
{
    wstring wstrMapName;
    UINT uMapSizeX;
    UINT uMapSizeY;
    eTERRAIN_INFO eTerrain;
    Ptr<CTexture> pMapTex;

    tMapData() : wstrMapName(), uMapSizeX(), uMapSizeY(), eTerrain(), pMapTex(nullptr)
    {}
};


class CStructBuffer;

class CCS_SCMapLoader :
    public CComputeShader
{
public:
    CCS_SCMapLoader();
    virtual ~CCS_SCMapLoader();

public:
    virtual bool BindDataCS() override;
    virtual void UnBindCS() override;

    void Debug();

private:
    CStructBuffer* m_arr_pSBufferTileSet[(int)eTERRAIN_INFO::END];

    //MXTM 관련
    //Map의 Chunk 파일 아래의 지형정보
    CStructBuffer* m_pSBuffer_MXTM;

    //============맵 정보=============
    tMapData m_tMapWorkSpace;




    //TODO : 나중에 맵 문제없이 로딩될경우 아래 변수는 삭제할것
    CStructBuffer* m_pSBuffer_Debug;
    tMtrlScalarData* m_DebugData;

public:
    bool LoadMap(const wstring& _wstrMapName, __out tMapData& _tMapData);

private:
    bool ReadMapData(char* Data, DWORD Size);
    bool PrepareDataCS();
};

