#include "pch.h"
#include "CUI_Menubar.h"


CUI_Menubar::CUI_Menubar(bool _bIsMainMenubar)
	: CUI("Menubar")
    , m_bMainMenuBar(_bIsMainMenubar)
{
}

CUI_Menubar::~CUI_Menubar()
{
}

void CUI_Menubar::finaltick()
{
    bool IsMenuBarClicked = false;

    //메뉴바 설정에 따라 다른 함수 호출
    if (true == m_bMainMenuBar)
        IsMenuBarClicked = ImGui::BeginMainMenuBar();
    else
    {
        IsMenuBarClicked = ImGui::BeginMenuBar();
    }


    //메뉴바가 클릭되었을 경우 여기서 로직을 진행
    if (true == IsMenuBarClicked)
    {
        //자신의 메뉴 렌더링을 해주고
        render_update();

        //자식 메뉴를 순회돌면서 렌더링
        LoopChildFinaltick();

        //메뉴바 종료
        if (true == m_bMainMenuBar)
            ImGui::EndMainMenuBar();
        else
            ImGui::EndMenuBar();
    }
}

