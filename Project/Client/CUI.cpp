#include "pch.h"
#include "CUI.h"

#include "CWidget.h"
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




void CUI::initRecursive()
{
	init();

	size_t size = m_vecChildUI.size();
	for (size_t i = 0; i < size; i++)
	{
		m_vecChildUI[i]->initRecursive();
	}
}


void CUI::tickRecursive()
{
	tick();

	size_t size = m_vecChildUI.size();
	for (size_t i = 0; i < size; i++)
	{
		m_vecChildUI[i]->tickRecursive();
	}
}




void CUI::finaltick()
{
	string strFullName = m_strName + m_strID;

	//�θ� UI �ּҰ� ���� ���, �� �ڽ��� �ֻ�� ������ UI�� ���
	if (nullptr == m_ParentUI)
	{
		//��޸��� â�� ���(�� UI�� ���� �־ �ٸ� UI �� ����� �� ���� ���)
		if (false == m_Modal)
		{
			//���� �Ϲ����� Begin �Լ��� �̿��ؼ� â�� �����Ѵ�.
			ImGui::Begin(strFullName.c_str(), &m_Active);

			

			//�ڽ��� ���� �����鿡 ���� ������ �۾��� �����ϰ�
			render_update();

			//�ڽ� ���� ��ҵ鿡 ���� ������Ʈ�� �����Ѵ�.
			LoopChildFinaltick();

			//�ڽ��� Parent UI �̹Ƿ� End()
			ImGui::End();
		}

		//���
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

	//�ڽ��� �ڽ� UI�� ���
	else
	{
		ImGui::BeginChild(strFullName.c_str(), m_vSize);

		render_update();

		LoopChildFinaltick();

		ImGui::EndChild();
	}
}
