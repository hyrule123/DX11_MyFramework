#include "pch.h"
#include "CImGuiMgr.h"

#include <Engine/CDevice.h>
#include <Engine/CKeyMgr.h>

#include <Engine/CPathMgr.h>


#include "CUI.h"
#include "CUI_BasicWIndow.h"
#include "CUIobj_Inspector.h"
#include "CUI_Menubar.h"
#include "CUI_Menu.h"
#include "CUI_MenuItem.h"
#include "CUI_Button.h"
#include "CUIobj_TestWindow.h"

#include "CUIObj_SCPrefabEditor.h"

#include <fstream>

#include "CUIobj_Contents.h"
#include "CUIobj_Outliner.h"
#include "CUIobj_AnimEditor.h"


CImGuiMgr::CImGuiMgr()
    : m_hWnd()
    , m_MainMenubar()
    , m_OpenWindowsMenu()
    , m_clear_color(0.45f, 0.55f, 0.60f, 1.00f)
    , m_bShowDemoWindow1()
    , m_SavedUIData()
    , m_bUIUpdated(true)
{
}

CImGuiMgr::~CImGuiMgr()
{
    std::filesystem::path origDir = CPathMgr::GetInst()->GetPathRel_Content();
    origDir /= DIRECTORY_NAME::SAVED_SETTING;
    std::filesystem::path fullPath = origDir / "imgui.ini";
    ImGui::SaveIniSettingsToDisk(fullPath.string().c_str());

    vector<CUI*> vecUI;

    //UI를 제거할 때 map에 있는 자신의 이터레이터를 제거하기 때문에
    //순회 돌면서 제거하면 에러가 발생한다.
    //그래서 별도의 vecUI에 UI 주소를 넣어 놓고 순회를 마친 후 다시 vecUI를 순회돌면서 제거해준다.
    for (const auto& iter : m_mapUI)
    {
        assert(nullptr != iter.second);

        if (nullptr != iter.second->GetParent())
            continue;

        iter.second->SaveRecursive(m_SavedUIData);

        vecUI.push_back(iter.second);
    }

    //auto iter = m_mapUI.begin();
    //while (iter != m_mapUI.end())
    //{
    //    iter = m_mapUI.erase(iter);
    //}

    
    m_mapUI.clear();

    size_t size = vecUI.size();
    for (size_t i = 0; i < size; ++i)
    {
        SAFE_DELETE(vecUI[i]);
    }

    fullPath = origDir;
    fullPath /= "ImGuiSave.json";
    std::ofstream fout(fullPath);
    if (true == fout.is_open())
    {
        fout << m_SavedUIData;
        fout.close();
    }


	// ImGui Release
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

CUI* CImGuiMgr::FindUI(const string& _UIName)
{
    auto iter = m_mapUI.find(_UIName);
    const auto& iterEnd = m_mapUI.end();

    if (iter != iterEnd)
        return iter->second;
        
    //키값으로 못찾았을 시 중복될 수도 있는 일반 이름으로 다시 한번 탐색
    for (iter = m_mapUI.begin(); iter != iterEnd; ++iter)
    {
        if (_UIName == iter->second->GetName())
            return iter->second;
    }

    //여기서도 못찾았으면 없는것임
    return nullptr;
}

void CImGuiMgr::AddUI(CUI* _pUI)
{
    assert(nullptr != _pUI);

    //같은 이름의 UI가 이미 존재하는지 확인.
    //존재할 경우 이름에 ID를 붙여 고유한 ID 생성
    const auto& iter = m_mapUI.find(_pUI->GetStrID());

    if (iter != m_mapUI.end())
    {
        _pUI->MakeUniqueID();

        //고유 이름을 만든 경우에는 저장의 의미가 없어짐
        //만약 고유 이름을 만들었는데 해당 창이 윈도우 속성을 가지고 있을경우 정보를 저장하지 않도록 설정한다.
        //웬만하면 중복된 창이 만들어질 가능성이 있는 창들을 애초에 저장을 안하도록 설정하는게 좋음
        CUI_BasicWindow* pWnd = dynamic_cast<CUI_BasicWindow*>(_pUI);
        if (nullptr != pWnd)
        {
            pWnd->AddImGuiWindowFlags(ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings);
        }
    }


    m_mapUI.insert(make_pair(_pUI->GetStrID(), _pUI));
    _pUI->init();
    _pUI->LoadRecursive(m_SavedUIData);

    //최상위 루트 UI일 경우 메인메뉴 갱신해야된다고 알림
    if (nullptr == _pUI->GetParent())
        m_bUIUpdated = true;
}


void CImGuiMgr::init(HWND _hWnd)
{
    ImGuiInit(_hWnd);
    CreateDefaultUI();
}

void CImGuiMgr::progress()
{
	begin();
	tick();
	finaltick();
	render();
}

void CImGuiMgr::ImGuiInit(HWND _hWnd)
{
    m_hWnd = _hWnd;
    //
    // 
    //IMGUI 초기화
    //
    //
    //
    //모든 초기화 과정은 IMGUI 예시 코드에서 가져온 것임. 모르겠으면 해당 코드를 보고 참고할것.
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    //io.ConfigViewportsNoDefaultParent = true;
    //io.ConfigDockingAlwaysTabBar = true;
    //io.ConfigDockingTransparentPayload = true;
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();

    //IMGUI가 메인창에 붙지 않도록 설정(각자의 뷰포트를 가지도록)
    io.ConfigViewportsNoAutoMerge = true;


    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    //설정 파일들 로드
    std::filesystem::path origDir = CPathMgr::GetInst()->GetPathRel_Content();
    
    origDir /= DIRECTORY_NAME::SAVED_SETTING;
    std::filesystem::path fullPath = origDir / "imgui.ini";
    io.IniFilename = NULL;

    ImGui::LoadIniSettingsFromDisk(fullPath.string().c_str());

    fullPath.clear();
    fullPath = origDir;
    fullPath /= "ImGuiSave.json";

    std::ifstream loadfile(fullPath);
    if (true == loadfile.is_open())
    {
        loadfile >> m_SavedUIData;
        loadfile.close();
    }



    //wstring path = CPathMgr::GetInst()->GetContentAbsPathW();
    //path += L"font/NotoSansKR-Regular.otf";
    //string fontpath;
    //::ConvertUnicodeToUTF8(path, fontpath);
    //
    //io.Fonts->AddFontFromFileTTF(fontpath.c_str(), 20.f, 0, io.Fonts->GetGlyphRangesKorean());


    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(_hWnd);
    CDevice* pDevice = CDevice::GetInst();
    ImGui_ImplDX11_Init(pDevice->GetDevice(), pDevice->GetDeviceContext());

}

void CImGuiMgr::CreateDefaultUI()
{
    m_MainMenubar = new CUI_Menubar("MainMenu", true);
    m_OpenWindowsMenu = m_MainMenubar->AddMenu("Open Windows");
    AddUI(m_MainMenubar);

    m_TestWindow = new CUIobj_TestWindow;
    AddUI(m_TestWindow);

    AddUI(new CUIobj_Inspector);
    CUIobj_Outliner* Outliner = new CUIobj_Outliner;
    Outliner->SetSaveEnable(true);
    AddUI(Outliner);
}

void CImGuiMgr::begin()
{
    //렌더링하기 전 IMGUI도 업데이트 해 준다.
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void CImGuiMgr::tick()
{
    if (true == m_bUIUpdated)
    {
        UpdateMainMenu();
        m_bUIUpdated = false;
    }

    for (const auto& pair : m_mapUI)
    {
        if (nullptr != pair.second->GetParent())
            continue;

        pair.second->tickRecursive();
    }
}

void CImGuiMgr::finaltick()
{
    //Demo UI
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    //if(m_bShowDemoWindow1)
        //ImGui::ShowDemoWindow(&m_bShowDemoWindow1);

    //// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    //{
    //    static float f = 0.0f;
    //    static int counter = 0;

    //    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

    //    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    //    ImGui::Checkbox("Demo Window", &m_bShowDemoWindow1);      // Edit bools storing our window open/close state
    //    ImGui::Checkbox("Another Window", &m_bShowDemoWindow2);

    //    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    //    ImGui::ColorEdit3("clear color", (float*)&m_clear_color); // Edit 3 floats representing a color

    //    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
    //        counter++;
    //    ImGui::SameLine();
    //    ImGui::Text("counter = %d", counter);

    //    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    //    ImGui::End();
    //}

    //// 3. Show another simple window.
    //if (m_bShowDemoWindow2)
    //{
    //    ImGui::Begin("Another Window", &m_bShowDemoWindow2);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    //    ImGui::Text("Hello from another window!");
    //    if (ImGui::Button("Close Me"))
    //        m_bShowDemoWindow2 = false;
    //    ImGui::End();
    //}


    for (const auto& pair : m_mapUI)
    {
        //비활성화 상태 또는 최상위 UI가 아닐 경우 continue
        if (nullptr != pair.second->GetParent())
            continue;
            
         pair.second->finaltick();
    }

    //엔터키를 누를 시 IMGUI 윈도우에 대한 포커스 해제
    if (KEY_DOWN(eKEY::ENTER))
        ImGui::SetWindowFocus(nullptr);
}


//렌더링은 finaltick에서 쌓아놓은 레이아웃을 토대로 ImGui에서 일괄적으로 진행함.
void CImGuiMgr::render()
{
    // ImGui Rendering
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // Update and Render additional Platform Windows
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void CImGuiMgr::UpdateMainMenu()
{
    if (nullptr == m_MainMenubar)
        return;

    m_OpenWindowsMenu->ClearChildUI();

    for (const auto& iter : m_mapUI)
    {
        if (nullptr == iter.second->GetParent())
        {
            const string& UIName = iter.second->GetName();

            if (UIName == "MainMenu")
                continue;

            CUI_MenuItem* pMenuItem = m_OpenWindowsMenu->AddMenuItem(UIName,
                tPtrData{ iter.second, } );
            pMenuItem->SetCallback(std::bind(&CImGuiMgr::OpenWindowsCallback, this,
                std::placeholders::_1, std::placeholders::_2));
        }
    }
}

void CImGuiMgr::OpenWindowsCallback(CUI_MenuItem* _pMenuItem, tPtrData _pData)
{
    CUI* pUI = static_cast<CUI*>(_pData.ptr);

    if (nullptr == _pMenuItem || nullptr == pUI)
        return;

    pUI->SetActive(true);
}
