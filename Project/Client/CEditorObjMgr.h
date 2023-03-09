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
    //디버그용 게임오브젝트를 모양별로 하나씩 만들어 놓고 모든 게임오브젝트를 해당 디버그용 오브젝트로 출력할 예정임.
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

