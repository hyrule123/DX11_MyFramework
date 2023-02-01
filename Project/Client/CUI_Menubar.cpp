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

    //�޴��� ������ ���� �ٸ� �Լ� ȣ��
    if (true == m_bMainMenuBar)
        IsMenuBarClicked = ImGui::BeginMainMenuBar();
    else
    {
        IsMenuBarClicked = ImGui::BeginMenuBar();
    }


    //�޴��ٰ� Ŭ���Ǿ��� ��� ���⼭ ������ ����
    if (true == IsMenuBarClicked)
    {
        //�ڽ��� �޴� �������� ���ְ�
        render_update();

        //�ڽ� �޴��� ��ȸ���鼭 ������
        LoopChildFinaltick();

        //�޴��� ����
        if (true == m_bMainMenuBar)
            ImGui::EndMainMenuBar();
        else
            ImGui::EndMenuBar();
    }
}

