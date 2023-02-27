#pragma once
#include "CComputeShader.h"

#include "ptr.h"
#include "CTexture.h"

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
    //�۾��� �ؽ�ó ���� �ּ�
    Ptr<CTexture> m_pTexture;

    Chunk m_MapDataChunk[(int)eSCMAP_DATA_TYPE::END];

    //============�� ����=============
    int m_MapSizeX;
    int m_MapSizeY;
    TerrainInfo m_Terrain;

    //�ε� �Ϸ� ��Ͽ�
    unsigned int m_LoadRef;
    unsigned int m_LoadCheck;

    //Ÿ�ϸ�
    //CSharedPtr<class CTileMapComponent> m_TileMap;

    //Map�� Chunk ���� �Ʒ��� ��������
    //CV5 �ּ�
    //VX4 �ּ�
    //VF4 �ּ�
    //VR4 �ּ�
    //WPE �ּ�

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