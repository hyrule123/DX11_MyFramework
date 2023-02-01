#include "pch.h"
#include "CUI_Menubar.h"


CUI_Menubar::CUI_Menubar(bool _bIsMainMenubar)
	: CUI_BasicWindow("Menubar")
    , m_bMainMenuBar(_bIsMainMenubar)
{
}

CUI_Menubar::~CUI_Menubar()
{
}

bool CUI_Menubar::beginUI()
{
    //�޴��� ������ ���� �ٸ� �Լ� ȣ��
    if (true == m_bMainMenuBar)
        return ImGui::BeginMainMenuBar();
    else
    {
        return ImGui::BeginMenuBar();
    }
}

void CUI_Menubar::endUI()
{
    if (true == m_bMainMenuBar)
        ImGui::EndMainMenuBar();
    else
        ImGui::EndMenuBar();
}


