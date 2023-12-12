#pragma once
//CCS_SCMapLoader과 cCom_Renderer_TilemapSC 쉐이더에서 공유하는 구조체들을 모아놓은 헤더
#include <EngineBase/Engine/defines.h>
#include <memory>

#include <UserShader/SCTilemapLoader.hlsli>

namespace scMap
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
    enum class eTilesetInfo : UINT32
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

    constexpr const char* eTilesetInfo_String[(int)eTilesetInfo::END] =
    {
        "badlands",
        "platform",
        "install",
        "ashworld",
        "jungle",
        "Desert",
        "Ice",
        "Twilight",
    };



    enum class eTilesetMember
    {
        CV5 = Register_t_SB_CV5,
        VX4 = Register_t_SB_VX4,
        VF4 = Register_t_SB_VF4,
        VR4 = Register_t_SB_VR4,
        WPE = Register_t_SB_WPE,
        END
    };

    struct tTileset
    {
        tCV5 cv5[CV5_MAX];
        tVX4 vx4[VX4_MAX];
        tVF4 vf4[VF4_MAX];
        tVR4 vr4[VR4_MAX];
        tWPE wpe[WPE_MAX];
    };


    //맵데이터로부터 로드해야하는 데이터들의 플래그
    enum class eSCmapDataType
    {
        TERRAIN,
        MAPSIZE,
        TILEMAP_ATLAS,
        UNIT_PLACEMENT,
        END
    };

    constexpr const char* strKey_SCMap[(int)eSCmapDataType::END] = {
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

    struct tMapRawData
    {
        std::string Data;

        //실제로 읽은 바이트 수
        DWORD ActualReadByte;
    };
}


