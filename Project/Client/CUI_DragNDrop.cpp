#include "pch.h"

//하나의 헤더를 공유함
#include "CUI_DragNDrop.h"



CUI_DragNDropSender::CUI_DragNDropSender(const string& _strKeySend)
	: CUI_Widget("DragNDrop", eWIDGET_TYPE::DRAG_N_DROP_SENDER)
	, m_strKeySend(_strKeySend)
	, m_flag()
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
	tPtrData* pData = new tPtrData;
	memset(pData, 0, sizeof(tPtrData));
	(*pData) = GetPtrData();
	if(ImGui::SetDragDropPayload(m_strKeySend.c_str(), pData, sizeof(tPtrData)))
		ImGui::Text(m_strKeySend.c_str());
	//위젯 Child를 넣을 경우 드래그 앤 드랍을 할 때 Description을 보여줄 수 있음. 
}

void CUI_DragNDropSender::endUI()
{
	ImGui::EndDragDropSource();
}




CUI_DragNDropReceiver::CUI_DragNDropReceiver(const string& _strKeyReceive)
	: CUI_Widget("Receiver", eWIDGET_TYPE::DRAN_N_DROP_RECEIVER)
	, m_strKeyReceive(_strKeyReceive)
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
		tPtrData data = {};
		memcpy(&data, payload->Data, payload->DataSize);
		SetDataPtr(data);
	}
}

void CUI_DragNDropReceiver::endUI()
{
	ImGui::EndDragDropTarget();
}
