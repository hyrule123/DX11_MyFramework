#include "PCH_Client.h"

#include <EngineBase/Engine/GameMainWindow.h>
#include <UserContents/UserContentsInitializer.h>

inline void DebugCheck(long _block);

BOOL APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    DebugCheck(0);

    

    tDesc_GameMainWindow Desc{};
    Desc.Inst = hInstance;
    Desc.TitleName = TEXT("StarCraft");
    Desc.ClassName = TEXT("GameEngine Client");
    Desc.LeftPos = 100;
    Desc.TopPos = 100;
    Desc.Width = 960;
    Desc.Height = 720;

    Desc.WindowIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

    Desc.ExternalInitFuncs.push_back(ehw::UserContentsInitializer::Init);

    Desc.GPUDesc.ResolutionX = 640u;
    Desc.GPUDesc.ResolutionY = 480u;
    Desc.GPUDesc.RefreshRate = 144u;

    return GameMainWindow::Run(Desc);
}

inline void DebugCheck(long _block)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    if (_block)
    {
        _CrtSetBreakAlloc(_block);
    }
}
