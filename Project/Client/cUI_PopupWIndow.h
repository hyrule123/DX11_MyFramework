#pragma once
#include "cUI_BasicWindow.h"
class cUI_PopupWindow :
    public cUI_BasicWindow
{
public:
	cUI_PopupWindow();
	virtual ~cUI_PopupWindow();

public:
	virtual bool beginUI() override;
	virtual void endUI() override;


private:
	ImGuiPopupFlags m_PopupFlags;

	ImVec2			m_vPopupPos;	// UI 위치
	bool			m_bModal;		// 모달 or 모달리스



public:
	void SetModal(bool _Modal) { m_bModal = _Modal; }
	bool IsModal() { return m_bModal; }

	ImVec2 GetPopupPos() { return m_vPopupPos; }
	void SetPopupPos(ImVec2 _vPos) { m_vPopupPos = _vPos; }



};

