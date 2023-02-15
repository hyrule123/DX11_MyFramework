#include "pch.h"
#include "CUI_PopupWIndow.h"

CUI_PopupWindow::CUI_PopupWindow()
	: CUI_BasicWindow("PopupWindow")
	, m_PopupFlags()
	, m_vPopupPos()
	, m_bModal()
{
	ImGuiWindowFlags flag = ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings;
	AddImGuiWindowFlags(flag);
}

CUI_PopupWindow::~CUI_PopupWindow()
{
}

bool CUI_PopupWindow::beginUI()
{


	////부모 UI 주소가 없을 경우, 즉 자신이 최상단 계층의 UI일 경우
//if (nullptr == m_ParentUI)
//{
//	//모달리스 창일 경우(이 UI가 열려 있어도 다른 UI 를 사용할 수 있을 경우)
//	if (false == m_bModal)
//	{
//		//가장 일반적인 Begin 함수를 이용해서 창을 생성한다.
//		ImGui::Begin(strFullName.c_str(), &m_Active);

//		

//		//자신의 구성 위젯들에 대한 렌더링 작업을 진행하고
//		render_update();

//		//자식 구성 요소들에 대한 업데이트도 진행한다.
//		LoopChildFinaltick();

//		//자신이 Parent UI 이므로 End()
//		ImGui::End();
//	}

//	//모달
//	else
//	{

//	}
//}

////자신이 자식 UI일 경우
//else
//{
//	ImGui::BeginChild(strFullName.c_str(), m_vSize);

//	render_update();

//	LoopChildFinaltick();

//	ImGui::EndChild();
//}

//endUI();

	ImGui::SetNextWindowPos(m_vPopupPos);
	ImGui::SetNextWindowSize(GetSize());

	ImGui::OpenPopup(GetStrID().data(), m_PopupFlags);

if (true == m_bModal)
{
	return ImGui::BeginPopupModal(GetStrID().data(), GetActivePtr(), GetImGuiWindowFlag());
}

	return true;
}

void CUI_PopupWindow::endUI()
{
	ImGui::EndPopup();
}
