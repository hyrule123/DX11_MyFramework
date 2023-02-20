#pragma once
#include "CSingleton.h"

#include "ptr.h"

class CTexture;

struct tAnim2D
{
    Vec2                m_vBackSize;
    Ptr<CTexture>       m_AtlasTex;
    vector<tAnim2DFrm>  m_vecFrm;
};

class CAnimMgr :
    public CSingleton<CAnimMgr>
{
    SINGLETON(CAnimMgr)

public:
    void init();

private:
    unordered_map<string, tAnim2D> m_mapAnim;

public:
    void Create(const string& _strAnimName, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _vBackSize, int _FrameCount, int _FPS);
    //const tAnim2DFrm& GetCurFrame() { return m_vecFrm[m_iCurFrm]; }
    //Vec2 GetBackSize() { return m_vBackSize; }
    //Ptr<CTexture> GetAtlasTex() { return m_AtlasTex; }
};

