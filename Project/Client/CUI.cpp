#include "pch.h"
#include "CUI.h"

CUI::CUI(const string& _ID)
	: m_strID(_ID)
	, m_ParentUI(nullptr)
	, m_Modal(false)
	, m_Active(true)
{

}

CUI::~CUI()
{
	size_t size = m_vecChildUI.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (nullptr != m_vecChildUI[i])
			delete(m_vecChildUI[i]);
	}
}




void CUI::finaltick()
{
	string strFullName = m_strName + m_strID;

	//부모 UI 주소가 없을 경우, 즉 자신이 최상단 계층의 UI일 경우
	if (nullptr == m_ParentUI)
	{
		//모달리스 창일 경우(이 UI가 열려 있어도 다른 UI 를 사용할 수 있을 경우)
		if (false == m_Modal)
		{
			//가장 일반적인 Begin 함수를 이용해서 창을 생성한다.
			ImGui::Begin(strFullName.c_str(), &m_Active);

			render_update();

			LoopChildFinaltick();

			//자신이 Parent UI 이므로 End()
			ImGui::End();
		}

		//모달
		else
		{
			ImGui::SetNextWindowPos(m_vPopupPos);
			ImGui::SetNextWindowSize(m_vSize);

			ImGui::OpenPopup(strFullName.c_str());
			if (ImGui::BeginPopupModal(strFullName.c_str(), &m_Active))
			{
				render_update();

				LoopChildFinaltick();
			}

			ImGui::EndPopup();
		}
	}

	//자신이 자식 UI일 경우
	else
	{
		ImGui::BeginChild(strFullName.c_str(), m_vSize);

		render_update();

		LoopChildFinaltick();

		ImGui::EndChild();
	}
}

