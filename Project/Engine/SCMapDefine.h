#pragma once

//CCS_SCMapLoader과 CTilemapComplete 쉐이더에서 공유하는 구조체들을 모아놓은 헤더


#include "ptr.h"
#include "S_H_SCMapLoader.hlsli"

struct tUnitData
{
    UINT32 UnitInfo;
    UINT16 PosX;
    UINT16 PosY;
    UINT16 ID;          //유닛 ID
    UINT16 Unknown;         //알려지지 않음
    UINT16 Property;    //유닛 속성(아래 참고 - 아직 사용하지는 않음)

    UINT16 UpgradableProperty;  //유닛의 상태변화에 적용가능한 속성(체력통 증가, 마나통 증가 등등) - 사용 X

    UINT8 Owner;
    UINT8 HPPercent;
    UINT8 ShieldPercent;
    UINT8 ManaPercent;

    UINT16 Resources;
    UINT16 NumUnitInContainer;

    UINT16 Unknown;
    UINT8  CurState;    //클로킹, 버로우 등 현재 상태(비트 마스크)

    UINT8 Padding;
};

/*
* https://cafe.daum.net/rpgguild/6cWR/158
    0000 = 유닛에 적용가능한 속성

    xxxx xxxx xxxE DCBA(2)



    A - 클로킹 칸 허용
    B - 버로우 칸 허용
    C - 띄우기 칸 허용
    D - 환상 칸 허용
    E - 무적 칸 허용
    x - 알려지지 않음 / 사용되지 않음



    0000 = 유닛의 현재상태값의 수치변화에 적용가능한 기능

    xxxx xxxx xxFE DCBA(2)



    A - 유닛 소유 기능
    B - 체력 변화 기능
    C - 쉴드 변화 기능
    D - 마나 변화 기능
    E - 자원의 양 변화 기능
    F - 적재소 양 변화 가능(리버, 캐리어)
    x - 알려지지 않음 / 사용되지 않음
*/

class CTexture;
struct tMapData
{
    string strMapName;
    UINT uMapSizeX;
    UINT uMapSizeY;
    eTILESET_INFO eTileSet;
    Ptr<CTexture> pMapTex;
    vector<tMegaTile> vecMegaTile;
    vector<tMiniTile> vecMiniTile;
    

    tMapData() : strMapName(), uMapSizeX(), uMapSizeY(), eTileSet()
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


class CStructBuffer;
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
    UNIT_PLACEMENT,
    END
};

constexpr const char* strKey_SCMap[(int)eSCMAP_DATA_TYPE::END] = {
    "ERA", "DIM", "MTXM", "UNIT"
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
