#pragma once
#include "CComputeShader.h"

#include "ptr.h"
#include "CTexture.h"

//"ERA" ���� ���� 
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


//�ʵ����ͷκ��� �ε��ؾ��ϴ� �����͵��� �÷���
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
    //�۾��� �ؽ�ó ���� �ּ�
    Ptr<CTexture> m_pTexture;

    Chunk m_MapDataChunk[(int)eSCMAP_DATA_TYPE::END];

    //============�� ����=============
    int m_MapSizeX;
    int m_MapSizeY;
    TerrainInfo m_Terrain;

    //�ε� �Ϸ� ��Ͽ� 
    UINT8 m_LoadRef;
    UINT8 m_LoadCheck;

    //Ÿ�ϸ�
    //CSharedPtr<class CTileMapComponent> m_TileMap;

    //Map�� Chunk ���� �Ʒ��� ��������
    
    CStructBuffer* m_pSBuffer_MXTM;

    //CV5 �ּ�
    CStructBuffer* m_pSBuffer_CV5;

    //VX4 �ּ�
    CStructBuffer* m_pSBuffer_VX4;

    
    //VF4 �ּ�
    CStructBuffer* m_pSBuffer_VF4;


    //VR4 �ּ�
    CStructBuffer* m_pSBuffer_VR4;


    //WPE �ּ�
    CStructBuffer* m_pSBuffer_WPE;

    CStructBuffer* m_pSBuffer_Debug;
    tMtrlScalarData* m_DebugData;

public:
    Ptr<CTexture> GetMap() const { return m_pTexture; }

private:
    void ReadMapData(char* Data, DWORD Size);
    void ResetMapData();
    bool LoadTileMap();
    bool LoadComplete();

};

inline bool CCS_SCMapLoader::LoadComplete()
{
    return (m_LoadCheck == m_LoadRef);
}