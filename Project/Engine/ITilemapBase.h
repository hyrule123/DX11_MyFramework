#pragma once
#include "IRenderer.h"

#include "S_H_TilemapAtlas.hlsli"

// ===============================
// ITilemapBase
// RS_TYPE : CULL_BACK
// DS_TYPE : LESS(Default)
// BS_TYPE : MASK

// Parameter
// g_CBuffer_Mtrl_Scalar.INT_0 : Tile X Count
// g_CBuffer_Mtrl_Scalar.int_1 : Tile Y Count
// g_tex_0 : Tile Atlas cTexture
//===============================

enum class eTILEMAP_TYPE
{
    ATLAS,
    COMPLETE
};

#define TileXCount INT_0
#define TileYCount INT_1
#define TileTex    TEX_0

class cStructBuffer;
class cCom_Camera;

class ITilemapBase :
    public IRenderer
{
public:
    ITilemapBase(eTILEMAP_TYPE _eTilemapType);
    virtual ~ITilemapBase();

    ITilemapBase(ITilemapBase const& _other);
    CLONE_DISABLE(ITilemapBase);

public:
    virtual void Init() override;
    virtual void FinalTick() override;
    virtual eRENDER_RESULT Render() override;

    //타일맵 종류별로 타일맵 갯수를 받으면 추가적으로 해야할 작업이 있으므로 가상함수로 선언했음.
    virtual void SetTileCount(UINT _iXCount, UINT _iYCount) { m_uTileCountX = _iXCount; m_uTileCountY = _iYCount; m_uNumTiles = m_uTileCountX * m_uTileCountY; }

    virtual bool SaveJson(Json::Value* _pJVal) override;
    virtual bool LoadJson(Json::Value* _pJVal) override;

private:
    eTILEMAP_TYPE       m_TilemapType;
    UINT                m_uTileCountX;  // 타일 가로
    UINT                m_uTileCountY;  // 타일 세로

    UINT                m_uNumTiles;

public:
    eTILEMAP_TYPE GetTilemapType() const { return m_TilemapType; }
    UINT GetTileCountX() const { return m_uTileCountX; }
    UINT GetTileCountY() const { return m_uTileCountY; }
    UINT GetNumTiles() const { return m_uNumTiles; }
};

