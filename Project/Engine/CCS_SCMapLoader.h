#pragma once
#include "CComputeShader.h"

#include "S_0_H_SCMapLoader.hlsli"

#include "SCMapDefine.h"


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
    tpSBufferTileSet m_arrpSBufferTileSet[(int)eTILESET_INFO::END];

    //MXTM 관련
    //Map의 tMapDataChunk 파일 아래의 지형정보
    CStructBuffer* m_pSBuffer_MXTM;

    //============맵 정보=============
    tMapData m_tMapWorkSpace;

    //TODO : 나중에 맵 문제없이 로딩될경우 아래 변수는 삭제할것
    CStructBuffer* m_pSBuffer_Debug;
    tMtrlScalarData* m_DebugData;

public:
    //Map 폴더 안의 맵 이름을 인자로 받아서 맵을 로드함.
    bool LoadMap(const string& _strMapName, __out tMapData& _tMapData);

private:
    bool ReadMapData(char* Data, DWORD Size);
    bool UploadMapDataToCS();
};

