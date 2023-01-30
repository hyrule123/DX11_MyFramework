#include "pch.h"
#include "CComboBox.h"



CComboBox::CComboBox()
	: CWidget("##ComboBox", eWIDGET_TYPE::COMBO_BOX)
	, m_iCurrentSelected(-1)
	, m_ComboFlags()
{
}

CComboBox::~CComboBox()
{
}


void CComboBox::tick()
{
	//인덱스가 유효하지 않은지 검사하고 유효하지 않을 시 -1로 변경
	if (false == IsIndexValid())
		m_iCurrentSelected = -1;
}

int CComboBox::render_update()
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
		//콤보박스를 클릭했다면 BeginCombo에서 true가 반환되므로
		CallCallbackFunc(eCALLBACK_TYPE::ONCLICK);


		size_t size = m_vecItem.size();
		for (int n = 0; n < size; ++n)
		{
			bool isSelected = (m_iCurrentSelected == n);

			//여기는 콤보박스에서 특정 값이 선택(클릭)되었을 떄 진입한다.
			if (true == ImGui::Selectable(m_vecItem[n].c_str(), isSelected))
			{
				m_iCurrentSelected = n;
				CallCallbackFunc(eCALLBACK_TYPE::ONSELECT);
			}
				

			if (isSelected) {}
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	return TRUE;
}

void CComboBox::SetCurrentSelected(const string& _SelectedName)
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