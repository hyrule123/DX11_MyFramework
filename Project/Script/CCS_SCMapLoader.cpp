#include "pch.h"
#include "CCS_SCMapLoader.h"





//일단 여기서 몰아서 성공킬예정
//나중에 지우고 분산시킬것
#include <Engine/cPathMgr.h>
#include <Engine/cStructBuffer.h>
#include <Engine/cTexture.h>
#include <Engine/cResMgr.h>
#include <Engine/cStructBuffer.h>
#include <Engine/cTexture.h>



using namespace SC_Map;

CCS_SCMapLoader::CCS_SCMapLoader()
    : m_arrpSBufferTileSet{}
    , m_pMapWorkSpace()
{


    
}

CCS_SCMapLoader::~CCS_SCMapLoader()
{
    for (int i = 0; i < (int)SC_Map::eTILESET_INFO::END; ++i)
    {
        SAFE_DELETE_ARRAY(m_arrpSBufferTileSet[i].arrTileSetMember);
    }
}

tNumDataCS CCS_SCMapLoader::BindDataCS()
{


}



shared_ptr<SC_Map::tMapData> CCS_SCMapLoader::LoadMap(const string_view _strMapName)
{


    //로드 실패 시 들어온 레퍼런스를 초기화하고 false 반환
    if (false == Execute())
        return nullptr;
            

    m_pMapWorkSpace->bMapLoaded = true;

    //성공 시 작업 정보를 넘겨주고 자신의 작업 영역은 초기화한다.
    shared_ptr<SC_Map::tMapData> tMapData = m_pMapWorkSpace;
    m_pMapWorkSpace = nullptr;
    return tMapData;
}






std::shared_ptr<tMapDataChunk> CCS_SCMapLoader::MultiThread_CopyChunk(const std::string& _dataStr, eSCMAP_DATA_TYPE _eDataType)
{
    using namespace std;
    shared_ptr<tMapDataChunk> pData = make_shared<tMapDataChunk>();

    //위치 찾기
    size_t pos = 0;
    pos = _dataStr.find(strKey_SCMap[(int)_eDataType]);

    if (pos == string::npos)
        return nullptr;

    {
        const char* adress = _dataStr.data() + pos;

        //이름 복사
        memcpy(pData->TypeName, adress, 4);

        //데이터 덩어리 사이즈 복사
        int* len = (int*)(adress + 4);
        pData->length = *len;

        pData->Data = new unsigned char[pData->length];
        memcpy(pData->Data, adress + 8, pData->length);
    }


    return pData;
}


