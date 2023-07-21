#pragma once
#include <Engine\Singleton.h>

//IMGUI
#include "ImGui/imgui.h"
#include "ImGui/imgui_stdlib.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"

#include "macroFunc_Imgui.h"

#include <Engine/jsoncpp.h>

#include "cUI_Structs.h"

//Delegate(함수 포인터
class cUI;
class cUIobj_Inspector;
class cUI_Menubar;
class cUI_Menu;
class cUI_MenuItem;
class cUI_BasicWindow;
class cUIobj_TestWindow;


typedef void (cUI::* UI_DELEGATE_0)(void);
typedef void (cUI::* UI_DELEGATE_1)(DWORD_PTR);
typedef void (cUI::* UI_DELEGATE_2)(DWORD_PTR, DWORD_PTR);

class cImGuiMgr
    : public Singleton<cImGuiMgr>
{
    SINGLETON(cImGuiMgr);
private:
    HWND                m_hWnd;
    unordered_map<string, cUI*>   m_mapUI;

    cUI_Menubar* m_MainMenubar;
    cUI_Menu* m_OpenWindowsMenu;

    cUIobj_TestWindow* m_TestWindow;

    // ImGui state
    ImVec4 m_clear_color;

    bool m_bShowDemoWindow1;
    //bool m_bShowDemoWindow

    Json::Value m_SavedUIData;

    bool m_bUIUpdated;

public:
    cUI* FindUI(const string& _UIName);
    const unordered_map<string, cUI*>& GetMapUI() const { return m_mapUI; }

    HWND GetHwnd() const { return m_hWnd; }
    void AddUI(cUI* _pUI);

    void ToggleDemoWindow() { m_bShowDemoWindow1 = !m_bShowDemoWindow1; }

    void RemoveUI(const string& _strID) { m_mapUI.erase(_strID); }

public:
    void init(HWND _hWnd);
    void progress();
    

private:
    void ImGuiInit(HWND _hWnd);
    void CreateDefaultUI();
    
    void begin();
    void Tick();
    void FinalTick();
    void render();

    void UpdateMainMenu();
    void OpenWindowsCallback(cUI_MenuItem* _pMenuItem, tPtrData _pData);

};

