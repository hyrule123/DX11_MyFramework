#pragma once
#include <Engine\CSingleton.h>

//IMGUI
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"



//Delegate(함수 포인터
class CUI;
class CInspectorUI;


typedef void (CUI::* UI_DELEGATE_0)(void);
typedef void (CUI::* UI_DELEGATE_1)(DWORD_PTR);
typedef void (CUI::* UI_DELEGATE_2)(DWORD_PTR, DWORD_PTR);

class CImguiMgr :
    public CSingleton<CImguiMgr>
{
    SINGLETON(CImguiMgr)
private:
    HWND                m_hWnd;
    map<string, CUI*>   m_mapUI;

    // ImGui state
    ImVec4 m_clear_color;

    bool m_bShowDemoWindow1;
    bool m_bShowDemoWindow2;

public:
    CUI* FindUI(const string& _UIName);
    HWND GetHwnd() const { return m_hWnd; }
    void CreateUI(CUI* _pUI);

public:
    void init(HWND _hWnd);
    void progress();
    

private:
    void CreateDefaultUI();
    

    void begin();
    void tick();
    void finaltick();
    void render();
};

