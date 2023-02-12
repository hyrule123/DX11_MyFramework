#include "pch.h"
#include "CImGuiMgr.h"

#include <Engine/CDevice.h>
#include <Engine/CKeyMgr.h>

#include <Engine/CPathMgr.h>


#include "CUI.h"
#include "CUI_Inspector.h"
#include "CUI_MainMenubar.h"

#include <fstream>

#include "JsonCPP.h"


CImGuiMgr::CImGuiMgr()
	: m_hWnd()
    , m_clear_color(0.45f, 0.55f, 0.60f, 1.00f)
    , m_bShowDemoWindow1()
    , m_SavedUIData()
{
}

CImGuiMgr::~CImGuiMgr()
{
    wstring origPath = CPathMgr::GetInst()->GetContentPath();
    origPath += L"SavedSettings/";
    wstring Path = origPath + L"imgui.ini";
    string utfpath = ::ConvertUnicodeToUTF8(Path);
    ImGui::SaveIniSettingsToDisk(utfpath.c_str());


    for (const auto& iter : m_mapUI)
    {
        assert(nullptr != iter.second);

        if (nullptr != iter.second->GetParent())
            continue;

        iter.second->SaveRecursive(m_SavedUIData);

        delete(iter.second);
    }

    Path.clear();
    Path = origPath + L"ImGuiSave.json";
    std::ofstream fout(Path);
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
    const auto& iter = m_mapUI.find(_UIName);

    if (iter == m_mapUI.end())
        return nullptr;
    
    return iter->second;
}

void CImGuiMgr::CreateUI(CUI* _pUI)
{
    m_mapUI.insert(make_pair(_pUI->GetName(), _pUI));
    _pUI->init();
    _pUI->LoadRecursive(m_SavedUIData);
}

void CImGuiMgr::init(HWND _hWnd)
{
    m_hWnd = _hWnd;
    //
    // 
    //IMGUI �ʱ�ȭ
    //
    //
    //
    //��� �ʱ�ȭ ������ IMGUI ���� �ڵ忡�� ������ ����. �𸣰����� �ش� �ڵ带 ���� �����Ұ�.
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
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }


    //���� ���ϵ� �ε�
    wstring origPath = CPathMgr::GetInst()->GetContentPath();
    origPath += L"SavedSettings/";
    wstring path = origPath + L"imgui.ini";
    string utfpath = ::ConvertUnicodeToUTF8(path);
    io.IniFilename = NULL;

    ImGui::LoadIniSettingsFromDisk(utfpath.c_str());

    path.clear();
    path = origPath + L"ImGuiSave.json";

    std::ifstream loadfile(path);
    if (true == loadfile.is_open())
    {
        loadfile >> m_SavedUIData;
        loadfile.close();
    }

    

    //wstring path = CPathMgr::GetInst()->GetContentPath();
    //path += L"font/NotoSansKR-Regular.otf";
    //string fontpath;
    //::ConvertUnicodeToUTF8(path, fontpath);
    //
    //io.Fonts->AddFontFromFileTTF(fontpath.c_str(), 20.f, 0, io.Fonts->GetGlyphRangesKorean());


    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(_hWnd);
    CDevice* pDevice = CDevice::GetInst();
    ImGui_ImplDX11_Init(pDevice->GetDevice(), pDevice->GetDeviceContext());




    CreateDefaultUI();
}

void CImGuiMgr::progress()
{
	begin();
	tick();
	finaltick();
	render();
}

void CImGuiMgr::CreateDefaultUI()
{
    CreateUI(new CUI_Inspector);

    CreateUI(new CUI_MainMenubar);
}

void CImGuiMgr::begin()
{
    //�������ϱ� �� IMGUI�� ������Ʈ �� �ش�.
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void CImGuiMgr::tick()
{
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
    if(m_bShowDemoWindow1)
        ImGui::ShowDemoWindow(&m_bShowDemoWindow1);

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
        //��Ȱ��ȭ ���� �Ǵ� �ֻ��� UI�� �ƴ� ��� continue
        if (nullptr != pair.second->GetParent())
            continue;
            
         pair.second->finaltick();
    }

    //����Ű�� ���� �� IMGUI �����쿡 ���� ��Ŀ�� ����
    if (KEY_TAP(KEY::ENTER))
        ImGui::SetWindowFocus(nullptr);
}


//�������� finaltick���� �׾Ƴ��� ���̾ƿ��� ���� ImGui���� �ϰ������� ������.
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
