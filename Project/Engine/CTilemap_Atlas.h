#pragma once
#include "CTilemap.h"
class CTilemap_Atlas :
    public CTilemap
{
public:
    CTilemap_Atlas();
    virtual ~CTilemap_Atlas();
    CLONE(CTilemap_Atlas);

public:
    virtual void init() override {};
    virtual void finaltick() override;
    virtual bool render() override;
    virtual void cleanup() override {};

    virtual void SetTileCount(UINT _iXCount, UINT _iYCount) override;

    

private:
    Vec2                m_vSliceSize;   // Ÿ�� �ϳ��� ũ��(UV ����)
    vector<tTile>       m_vecTile;
    CStructBuffer* m_SBuffer;

public:
    void SetSliceSize(Vec2 _vSliceSize) { m_vSliceSize = _vSliceSize; }

    void BindData();
};

