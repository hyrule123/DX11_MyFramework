#include "pch.h"
#include "CUI_ComboBox.h"


CUI_ComboBox::CUI_ComboBox()
	: CUI_Widget("##ComboBox", eWIDGET_TYPE::COMBO_BOX)
	, m_iCurrentSelected(-1)
	, m_ComboFlags()
{
}

CUI_ComboBox::~CUI_ComboBox()
{
}


void CUI_ComboBox::tick()
{
	//�ε����� ��ȿ���� ������ �˻��ϰ� ��ȿ���� ���� �� -1�� ����
	if (false == IsIndexValid())
		m_iCurrentSelected = -1;
}

int CUI_ComboBox::render_update()
{
	string Label;
	if (true == GetLeftLabel())
		Label += "##";
	Label += GetName();

	string Preview;
	if (true == IsIndexValid())
		Preview = m_vecItem[m_iCurrentSelected];

	if (true == ImGui::BeginCombo(Label.c_str(), Preview.c_str(), m_ComboFlags))
	{
		//�޺��ڽ��� Ŭ���ߴٸ� BeginCombo���� true�� ��ȯ�ǹǷ�
		CallCallbackFunc(eCALLBACK_TYPE::ONCLICK);


		size_t size = m_vecItem.size();
		for (int n = 0; n < size; ++n)
		{
			bool isSelected = (m_iCurrentSelected == n);

			//����� �޺��ڽ����� Ư�� ���� ����(Ŭ��)�Ǿ��� �� �����Ѵ�.
			if (true == ImGui::Selectable(m_vecItem[n].c_str(), isSelected))
			{
				m_iCurrentSelected = n;
				CallCallbackFunc(eCALLBACK_TYPE::ONSELECT);
			}
				

			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
				
		}

		ImGui::EndCombo();
	}

	return TRUE;
}

void CUI_ComboBox::SetCurrentSelected(const string& _SelectedName)
{
	size_t size = m_vecItem.size();
	for (size_t i = 0; i < size; i++)
	{
		if (_SelectedName == m_vecItem[i])
		{
			m_iCurrentSelected = (int)i;
			return;
		}
	}

	m_iCurrentSelected = -1;
}