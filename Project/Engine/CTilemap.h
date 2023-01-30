#pragma once
#include "CRenderComponent.h"


// ===============================
// Tilemap
// RS_TYPE : CULL_BACK
// DS_TYPE : LESS(Default)
// BS_TYPE : MASK

// Parameter
// g_int_0 : Tile X Count
// g_int_1 : Tile Y Count
// g_tex_0 : Tile Atlas Texture
//===============================

#define TileXCount INT_0
#define TileYCount INT_1
#define TileTex    TEX_0

class CStructBuffer;

class CTilemap :
    public CRenderComponent
{
public:
    CTilemap();
    virtual ~CTilemap();
    CLONE(CTilemap)

public:
    virtual void finaltick() override;
    virtual void render() override;
    virtual void cleanup() override {};

    void UpdateData();

private:
    UINT                m_iTileCountX;  // Ÿ�� ����
    UINT                m_iTileCountY;  // Ÿ�� ����
    Vec2                m_vSliceSize;   // Ÿ�� �ϳ��� ũ��(UV ����)
    vector<tTile>       m_vecTile;
    CStructBuffer*      m_SBuffer;

public:
    void SetTileCount(UINT _iXCount, UINT _iYCount);
    void SetSliceSize(Vec2 _vSliceSize) { m_vSliceSize = _vSliceSize; }

};

