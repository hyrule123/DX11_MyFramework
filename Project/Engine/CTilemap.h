#pragma once
#include "CRenderComponent.h"

#include "S_0_H_TilemapAtlas.hlsli"

// ===============================
// Tilemap
// RS_TYPE : CULL_BACK
// DS_TYPE : LESS(Default)
// BS_TYPE : MASK

// Parameter
// g_CBuffer_Mtrl_Scalar.INT_0 : Tile X Count
// g_CBuffer_Mtrl_Scalar.int_1 : Tile Y Count
// g_tex_0 : Tile Atlas Texture
//===============================

enum class eTILE_TYPE
{
    COMPLETE,   //�ϼ��Ǿ��ִ� Ÿ�ϸ�(�״�� ��¸��ϸ� �Ǵ� Ÿ�ϸ�)
    ATLAS       //��Ʋ�󽺿� Ÿ�ϸ��� ���� �׸��� ���ְ�, UV��ǥ�� ���ؼ� �׷����� Ÿ�ϸ�
};

#define TileXCount INT_0
#define TileYCount INT_1
#define TileTex    TEX_0

class CStructBuffer;
class CCamera;

class CTilemap :
    public CRenderComponent
{
private:
    CTilemap() = delete;
public:
    CTilemap(eTILE_TYPE _eTileType);
    virtual ~CTilemap();

public:
    virtual void init() = 0;
    virtual void finaltick() = 0;
    virtual bool render() = 0;

    virtual void SetTileCount(UINT _iXCount, UINT _iYCount) { m_uTileCountX = _iXCount; m_uTileCountY = _iYCount; }

private:
    eTILE_TYPE          m_eTileType;
    UINT                m_uTileCountX;  // Ÿ�� ����
    UINT                m_uTileCountY;  // Ÿ�� ����


public:
    
    UINT GetTileCountX() const { return m_uTileCountX; }
    UINT GetTileCountY() const { return m_uTileCountY; }
    
};

