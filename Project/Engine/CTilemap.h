#pragma once
#include "CRenderComponent.h"

#include "Shader_TilemapAtlas_0_Header.hlsli"

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
    NOT_SET,
    COMPLETE,   //완성되어있는 타일맵(그대로 출력만하면 되는 타일맵)
    ATLAS       //아틀라스에 타일맵의 개별 그림이 모여있고, UV좌표를 통해서 그려지는 타일맵
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
    CTilemap();
    virtual ~CTilemap();
    CLONE(CTilemap)

public:
    virtual void init() override; 
    virtual void finaltick() override;
    virtual bool render(eCAMERA_INDEX _eCamIdx) override;
    virtual void cleanup() override {};

    void BindData();

private:
    eTILE_TYPE          m_eTileType;
    UINT                m_iTileCountX;  // 타일 가로
    UINT                m_iTileCountY;  // 타일 세로
    Vec2                m_vSliceSize;   // 타일 하나의 크기(UV 단위)
    vector<tTile>       m_vecTile;
    CStructBuffer*      m_SBuffer;

public:
    void SetTileType(eTILE_TYPE _eType);

    
    void SetTileCount(UINT _iXCount, UINT _iYCount);
    void SetSliceSize(Vec2 _vSliceSize) { m_vSliceSize = _vSliceSize; }

};

