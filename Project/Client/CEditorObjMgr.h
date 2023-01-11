#pragma once

class CGameObject;

class CEditorObjMgr :
    public CSingleton<CEditorObjMgr>
{
private:
    //디버그용 게임오브젝트를 모양별로 하나씩 만들어 놓고 모든 게임오브젝트를 해당 디버그용 오브젝트로 출력할 예정임.
    CGameObject* m_arrDebugShape[eSHAPE_END];
    list<tDebugShapeInfo> m_listDebugShapeInfo;

public:
    


public:
    void init();
    void progress();

private:
    void tick();
    void render();
    

    SINGLETON(CEditorObjMgr);
};

