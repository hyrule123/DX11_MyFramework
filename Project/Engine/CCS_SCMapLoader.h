#pragma once
#include "CComputeShader.h"

#include "ptr.h"
#include "CTexture.h"

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

class CCS_SCMapLoader :
    public CComputeShader
{
public:
    CCS_SCMapLoader();
    virtual ~CCS_SCMapLoader();

public:
    virtual bool BindDataCS() override;
    virtual void UnBindCS() override;

private:
    //작업할 텍스처 영역 주소
    Ptr<CTexture> m_pTexture;

    Chunk m_MapDataChunk[(int)eSCMAP_DATA_TYPE::END];

    //============맵 정보=============
    int m_MapSizeX;
    int m_MapSizeY;
    TerrainInfo m_Terrain;

    //로드 완료 기록용
    unsigned int m_LoadRef;
    unsigned int m_LoadCheck;

    //타일맵
    //CSharedPtr<class CTileMapComponent> m_TileMap;

    //Map의 Chunk 파일 아래의 지형정보
    //CV5 주소
    //VX4 주소
    //VF4 주소
    //VR4 주소
    //WPE 주소

private:
    void ReadMapData(char* Data, DWORD Size);
    void ResetMapData();
    void LoadTileMap();
    bool LoadComplete();

};

inline bool CCS_SCMapLoader::LoadComplete()
{
    return (m_LoadCheck == m_LoadRef);
}