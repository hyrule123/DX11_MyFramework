#pragma once
#include "CComputeShader.h"

#include "ptr.h"
#include "CTexture.h"

//"ERA" 안의 정보 
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
    //작업할 텍스처 영역 주소
    Ptr<CTexture> m_pTexture;

    Chunk m_arrMapDataChunk[(int)eSCMAP_DATA_TYPE::END];

    //============맵 정보=============
    int m_MapSizeX;
    int m_MapSizeY;
    TerrainInfo m_Terrain;

    //로드 완료 기록용 
    /*UINT8 m_LoadRef;
    UINT8 m_LoadCheck;*/

    //타일맵
    //CSharedPtr<class CTileMapComponent> m_TileMap;

    //Map의 Chunk 파일 아래의 지형정보
    
    CStructBuffer* m_pSBuffer_MXTM;
    //CV5 주소
    CStructBuffer* m_pSBuffer_CV5;
    //VX4 주소
    CStructBuffer* m_pSBuffer_VX4;
    //VF4 주소
    CStructBuffer* m_pSBuffer_VF4;
    //VR4 주소
    CStructBuffer* m_pSBuffer_VR4;
    //WPE 주소
    CStructBuffer* m_pSBuffer_WPE;

    //TODO : 나중에 맵 문제없이 로딩될경우 아래 변수는 삭제할것
    CStructBuffer* m_pSBuffer_Debug;
    tMtrlScalarData* m_DebugData;

public:
    Ptr<CTexture> GetMap() const { return m_pTexture; }

private:
    bool ReadMapData(char* Data, DWORD Size);
    void ResetMapData();
    bool PrepareDataCS();
    bool LoadComplete();
};

inline bool CCS_SCMapLoader::LoadComplete()
{
    for (int i = 0; i < (int)eSCMAP_DATA_TYPE::END; ++i)
    {
        //할당된 데이터가 하나도 없을 경우 false 반환
        if (nullptr == m_arrMapDataChunk[i].Data)
            return false;
    }

    return true;
}