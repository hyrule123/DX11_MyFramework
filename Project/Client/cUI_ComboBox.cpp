#include "pch.h"
#include "cUI_ComboBox.h"


cUI_ComboBox::cUI_ComboBox(const string& _Name)
	: cUI_Widget(_Name, eWIDGET_TYPE::COMBO_BOX)
	, m_iCurrentSelected(-1)
	, m_ComboFlags()
	, m_fLeftLabelWidth(100.f)
	, m_fWidth(0.f)
{
}

cUI_ComboBox::~cUI_ComboBox()
{
}



bool cUI_ComboBox::beginUI()
{

	string Label;
	if (true == m_bLeftLabel)
	{
		ImGui::Text(GetStrID().data());
		ImGui::SameLine(m_fLeftLabelWidth);

		Label += "##";
	}
	Label += GetKey();

	if (true == m_bWidthSet)
		ImGui::PushItemWidth(m_fWidth);

	string Preview;
	if (true == IsIndexValid())
		Preview = m_vecItem[m_iCurrentSelected].strName;

	return ImGui::BeginCombo(Label.c_str(), Preview.c_str(), m_ComboFlags);
}


void cUI_ComboBox::render_update()
{
	//콤보박스를 클릭했다면 BeginCombo에서 true가 반환되므로
	CallCallbackFunc(eCALLBACK_TYPE::ONCLICK);


	size_t size = m_vecItem.size();
	for (int n = 0; n < size; ++n)
	{
		bool isSelected = (m_iCurrentSelected == n);

		//여기는 콤보박스에서 특정 값이 선택(클릭)되었을 떄 진입한다.
		if (true == ImGui::Selectable(m_vecItem[n].strName.c_str(), isSelected))
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

	if (true == m_bWidthSet)
		ImGui::PopItemWidth();
}

void cUI_ComboBox::endUI()
{

}

void cUI_ComboBox::Tick()
{
	//인덱스가 유효하지 않은지 검사하고 유효하지 않을 시 -1로 변경
	if (false == IsIndexValid())
		m_iCurrentSelected = -1;
}

void cUI_ComboBox::SetCurrentSelected(const string& _SelectedName)
{
	size_t size = m_vecItem.size();
	for (size_t i = 0; i < size; i++)
	{
		if (_SelectedName == m_vecItem[i].strName)
		{
			m_iCurrentSelected = (int)i;
			return;
		}
	}

	m_iCurrentSelected = -1;
}
