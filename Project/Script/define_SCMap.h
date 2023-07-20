#pragma once

//CCS_SCMapLoader과 cCom_Renderer_TilemapSC 쉐이더에서 공유하는 구조체들을 모아놓은 헤더

#include <Engine/Ptr.h>
#include "S_H_Struct.hlsli"

#include <HLSL/S_H_SCMapLoader.hlsli>

#include <Engine/cStructBuffer.h>
#include <Engine/cTexture.h>

namespace SC_Map
{
    struct tUnitData
    {
        UINT32 UnitInfo;
        UINT16 PosX;
        UINT16 PosY;
        UINT16 ID;          //유닛 ID
        UINT16 LinkedUnitFlag;         //애드온, 나이더스 커널 등 연결된 유닛 유무
        UINT16 Property;    //유닛 속성(아래 참고 - 아직 사용하지는 않음)

        UINT16 UpgradableProperty;  //유닛의 상태변화에 적용가능한 속성(체력통 증가, 마나통 증가 등등) - 사용 X

        UINT8 Owner;
        UINT8 HPPercent;
        UINT8 ShieldPercent;
        UINT8 EnergyPercent;

        UINT32 Resources;
        UINT16 NumUnitInContainer;

        UINT16 UnitStateFlag;
        UINT32 Unused;
        UINT32 LinkedUnitClassInst; //연결된 유닛(애드온, 나이더스 커널)의 인스턴스 정보
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

    //스타크래프트 맵 데이터 속 "ERA" 안에 들어있는 정보 
    enum class eTILESET_INFO : UINT32
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



    class ::cTexture;
    struct tMapData
    {
        UINT32 uNumMegatileX;
        UINT32 uNumMegatileY;
        eTILESET_INFO eTileSet;

        bool bMapLoaded;
        string strMapName;
        Ptr<cTexture> pMapTex;

        vector<tMegaTile> vecMegaTile;
        vector<tMiniTile> vecMiniTile;
        vector<tUnitData> vecUnitData;

        //MXTM 관련
        //Map의 SC_Map::tMapDataChunk 파일 아래의 지형정보
        cStructBuffer* pSBuffer_MXTM;
        cStructBuffer* pSBufferRW_Megatile;
        cStructBuffer* pSBufferRW_Minitile;

        tMapData() 
            : uNumMegatileX()
            , uNumMegatileY()
            , eTileSet()
            , bMapLoaded()
            , strMapName()
            , pMapTex()
            , vecMegaTile()
            , vecMiniTile()
            , vecUnitData()

            , pSBuffer_MXTM()
            , pSBufferRW_Megatile()
            , pSBufferRW_Minitile()
        {}

        ~tMapData()
        {
            SAFE_DELETE(pSBuffer_MXTM);
            SAFE_DELETE(pSBufferRW_Megatile);
            SAFE_DELETE(pSBufferRW_Minitile);
        }
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


    class ::cStructBuffer;
    struct tpSBufferTileSet
    {
        cStructBuffer* arrTileSetMember;
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
}


