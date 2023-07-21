#pragma once

#include <Engine/Singleton.h>

#include <Engine/S_H_Debug.hlsli>

class cGameObject;
class cCom_Camera;

class cScript_MouseCursor;
class cEditorObjMgr
    : public Singleton<cEditorObjMgr>
{
    SINGLETON(cEditorObjMgr);
public:
    void init();
    void progress();

private:
    void Tick();
    void render();

private:
    //디버그용 게임오브젝트를 모양별로 하나씩 만들어 놓고 모든 게임오브젝트를 해당 디버그용 오브젝트로 출력할 예정임.
    cGameObject* m_arrDebugShape[(UINT)eDEBUGSHAPE_TYPE::END];
    vector<tDebugShapeInfo> m_vecDebugShapeInfo;

    cGameObject* m_pEditorCam;

    cGameObject* m_pMousePicker;

    cScript_MouseCursor* m_MouseScript;
    


private:
    void CreateDebugShape();
    void CreateEditorCamera();

    cGameObject* GetSelectedObj();
    
    void MouseLBTNCallback(cGameObject* _pObj);

};

