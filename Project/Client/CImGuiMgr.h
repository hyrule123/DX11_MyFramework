#pragma once
#include <Engine\CSingleton.h>

//IMGUI
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"


#include <UtilLib_DLL/json/json.h>

#include "CUI_Structs.h"

//Delegate(함수 포인터
class CUI;
class CUIobj_Inspector;
class CUI_Menubar;
class CUI_Menu;
class CUI_MenuItem;
class CUI_BasicWindow;
class CUIobj_TestWindow;


typedef void (CUI::* UI_DELEGATE_0)(void);
typedef void (CUI::* UI_DELEGATE_1)(DWORD_PTR);
typedef void (CUI::* UI_DELEGATE_2)(DWORD_PTR, DWORD_PTR);

class CImGuiMgr :
    public CSingleton<CImGuiMgr>
{
    SINGLETON(CImGuiMgr)
private:
    HWND                m_hWnd;
    map<string, CUI*>   m_mapUI;

    CUI_Menubar* m_MainMenubar;
    CUI_Menu* m_OpenWindowsMenu;

    CUIobj_TestWindow* m_TestWindow;

    // ImGui state
    ImVec4 m_clear_color;

    bool m_bShowDemoWindow1;
    //bool m_bShowDemoWindow

    Json::Value m_SavedUIData;

    bool m_bUIUpdated;

public:
    CUI* FindUI(const string& _UIName);
    const map<string, CUI*>& GetMapUI() const { return m_mapUI; }

    HWND GetHwnd() const { return m_hWnd; }
    void AddUI(CUI* _pUI);

    void ToggleDemoWindow() { m_bShowDemoWindow1 = !m_bShowDemoWindow1; }

    void RemoveUI(const string& _strID) { m_mapUI.erase(_strID); }

public:
    void init(HWND _hWnd);
    void progress();
    

private:
    void ImGuiInit(HWND _hWnd);
    void CreateDefaultUI();
    
    void begin();
    void tick();
    void finaltick();
    void render();

    void UpdateMainMenu();
    void OpenWindowsCallback(CUI_MenuItem* _pMenuItem, tPtrData _pData);

};

