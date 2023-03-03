#include "pch.h"

//�ϳ��� ����� ������
#include "CUI_DragNDrop.h"



CUI_DragNDropSender::CUI_DragNDropSender()
	: CUI_Widget("DragNDrop", eWIDGET_TYPE::DRAG_N_DROP_SENDER)
	, m_flag()
	, m_tData{}
{
}

CUI_DragNDropSender::~CUI_DragNDropSender()
{
}

bool CUI_DragNDropSender::beginUI()
{
	return ImGui::BeginDragDropSource();
}

void CUI_DragNDropSender::render_update()
{
	ImGui::SetDragDropPayload(m_strKeySend.c_str(), m_tData.pData, m_tData.uDataSize);

	//���� Child�� ���� ��� �巡�� �� ����� �� �� Description�� ������ �� ����. 
}

void CUI_DragNDropSender::endUI()
{
	ImGui::EndDragDropSource();
}




CUI_DragNDropReceiver::CUI_DragNDropReceiver()
	: CUI_Widget("Receiver", eWIDGET_TYPE::DRAN_N_DROP_RECEIVER)
{
}

CUI_DragNDropReceiver::~CUI_DragNDropReceiver()
{
}

bool CUI_DragNDropReceiver::beginUI()
{
	return ImGui::BeginDragDropTarget();
}

void CUI_DragNDropReceiver::render_update()
{
	if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(m_strKeyReceive.c_str()))
	{

	}
}

void CUI_DragNDropReceiver::endUI()
{
	ImGui::EndDragDropTarget();
}