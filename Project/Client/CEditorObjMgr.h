#pragma once

class CGameObject;
class CCamera;

class CEditorObjMgr :
    public CSingleton<CEditorObjMgr>
{
private:
    //����׿� ���ӿ�����Ʈ�� ��纰�� �ϳ��� ����� ���� ��� ���ӿ�����Ʈ�� �ش� ����׿� ������Ʈ�� ����� ������.
    CGameObject* m_arrDebugShape[(UINT)eSHAPE_TYPE::END];
    vector<tDebugShapeInfo> m_vecDebugShapeInfo;

    CGameObject* m_pEditorCam;

public:
   

public:
    void init();
    void progress();

private:
    void tick();
    void render();

private:
    void CreateDebugShape();
    void CreateEditorCamera();
    

    SINGLETON(CEditorObjMgr);
};

