#pragma once

#include <Engine/CSingleton.h>

#include <Engine/S_0_H_Debug.hlsli>

class CGameObject;
class CCamera;

class CEditorObjMgr :
    public CSingleton<CEditorObjMgr>
{
    SINGLETON(CEditorObjMgr);

private:
    //����׿� ���ӿ�����Ʈ�� ��纰�� �ϳ��� ����� ���� ��� ���ӿ�����Ʈ�� �ش� ����׿� ������Ʈ�� ����� ������.
    CGameObject* m_arrDebugShape[(UINT)eDEBUGSHAPE_TYPE::END];
    vector<tDebugShapeInfo> m_vecDebugShapeInfo;

    CGameObject* m_pEditorCam;

    CGameObject* m_pMousePicker;

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
    
    void MouseLBTNCallback(CGameObject* _pObj);

};

