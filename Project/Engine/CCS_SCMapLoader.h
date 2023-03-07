#pragma once
#include "CComputeShader.h"

#include "Shader_SCMapLoader_0_Header.hlsli"

#include "ptr.h"
#include "CTexture.h"


class CStructBuffer;

//"ERA" ���� ���� 
enum class eTILESET_INFO : UINT8
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



//�ʵ����ͷκ��� �ε��ؾ��ϴ� �����͵��� �÷���
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


struct tMapData
{
    wstring wstrMapName;
    UINT uMapSizeX;
    UINT uMapSizeY;
    eTILESET_INFO eTileSet;
    Ptr<CTexture> pMapTex;


    tMapData() : wstrMapName(), uMapSizeX(), uMapSizeY(), eTileSet(), pMapTex(nullptr)
    {}
};




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

    //MXTM ����
    //Map�� tMapDataChunk ���� �Ʒ��� ��������
    CStructBuffer* m_pSBuffer_MXTM;

    //============�� ����=============
    tMapData m_tMapWorkSpace;

    //TODO : ���߿� �� �������� �ε��ɰ�� �Ʒ� ������ �����Ұ�
    CStructBuffer* m_pSBuffer_Debug;
    tMtrlScalarData* m_DebugData;

public:
    bool LoadMap(const wstring& _wstrMapName, __out tMapData& _tMapData);

private:
    bool ReadMapData(char* Data, DWORD Size);
    bool UploadMapDataToCS();
};

