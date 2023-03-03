#include "pch.h"

//하나의 헤더를 공유함
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

	//위젯 Child를 넣을 경우 드래그 앤 드랍을 할 때 Description을 보여줄 수 있음. 
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