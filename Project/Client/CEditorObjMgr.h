#pragma once

class CGameObject;

class CEditorObjMgr :
    public CSingleton<CEditorObjMgr>
{
private:
    //����׿� ���ӿ�����Ʈ�� ��纰�� �ϳ��� ����� ���� ��� ���ӿ�����Ʈ�� �ش� ����׿� ������Ʈ�� ����� ������.
    CGameObject* m_arrDebugShape[eSHAPE_END];
    vector<tDebugShapeInfo> m_vecDebugShapeInfo;

public:
    


public:
    void init();
    void progress();

private:
    void tick();
    void render();
    

    SINGLETON(CEditorObjMgr);
};

