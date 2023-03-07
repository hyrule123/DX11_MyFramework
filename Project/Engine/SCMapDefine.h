#pragma once

#include "ptr.h"
class CTexture;
struct tMapData
{
    wstring wstrMapName;
    UINT uMapSizeX;
    UINT uMapSizeY;
    eTILESET_INFO eTileSet;
    Ptr<CTexture> pMapTex;


    tMapData() : wstrMapName(), uMapSizeX(), uMapSizeY(), eTileSet()
    {}
};

enum class eTILESET_MEMBER
{
    CV5,
    VX4,
    VF4,
    VR4,
    WPE,
    END
};

struct tTileSet
{
    CV5 cv5[CV5_MAX];
    VX4 vx4[VX4_MAX];
    VF4 vf4[VF4_MAX];
    VR4 vr4[VR4_MAX];
    WPE wpe[WPE_MAX];
};

struct tpSBufferTileSet
{
    CStructBuffer* arrTileSetMember;
};



//맵데이터로부터 로드해야하는 데이터들의 플래그
enum class eSCMAP_DATA_TYPE
{
    TERRAIN,
    MAPSIZE,
    TILEMAP_ATLAS,
    END
};

typedef struct tMapDataChunk
{
    char TypeName[5];
    unsigned long length;
    unsigned char* Data;

    tMapDataChunk() :
        TypeName{},
        length(), Data()
    {
    }
    ~tMapDataChunk()
    {
        SAFE_DELETE_ARRAY(Data);
    }


} tMapDataChunk;
