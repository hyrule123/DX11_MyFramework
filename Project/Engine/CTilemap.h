#pragma once
#include "CRenderComponent.h"

#include "S_H_TilemapAtlas.hlsli"

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

enum class eTILEMAP_TYPE
{
    ATLAS,
    COMPLETE
};

#define TileXCount INT_0
#define TileYCount INT_1
#define TileTex    TEX_0

class CStructBuffer;
class CCamera;

class CTilemap :
    public CRenderComponent
{
public:
    CTilemap(eTILEMAP_TYPE _eTilemapType);
    virtual ~CTilemap();

public:
    virtual void init();
    virtual void finaltick() = 0;
    virtual bool render();

    virtual void SetTileCount(UINT _iXCount, UINT _iYCount) { m_uTileCountX = _iXCount; m_uTileCountY = _iYCount; }

    virtual bool SaveJson(Json::Value* _pJVal) override;
    virtual bool LoadJson(Json::Value* _pJVal) override;

private:
    eTILEMAP_TYPE       m_TilemapType;
    UINT                m_uTileCountX;  // 타일 가로
    UINT                m_uTileCountY;  // 타일 세로

public:
    eTILEMAP_TYPE GetTilemapType() const { return m_TilemapType; }
    UINT GetTileCountX() const { return m_uTileCountX; }
    UINT GetTileCountY() const { return m_uTileCountY; }
};

