#pragma once
#include "CRenderComponent.h"


// ===============================
// Tilemap
// RS_TYPE : CULL_BACK
// DS_TYPE : LESS(Default)
// BS_TYPE : MASK

// Parameter
// g_CBuffer_MtrlData.int_0 : Tile X Count
// g_CBuffer_MtrlData.int_1 : Tile Y Count
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

    void BindData();

private:
    UINT                m_iTileCountX;  // 타일 가로
    UINT                m_iTileCountY;  // 타일 세로
    Vec2                m_vSliceSize;   // 타일 하나의 크기(UV 단위)
    vector<tTile>       m_vecTile;
    CStructBuffer*      m_SBuffer;

public:
    void SetTileCount(UINT _iXCount, UINT _iYCount);
    void SetSliceSize(Vec2 _vSliceSize) { m_vSliceSize = _vSliceSize; }

};

