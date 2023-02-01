#pragma once
#include "CUI_BasicWIndow.h"
class CUI_PopupWindow :
    public CUI_BasicWindow
{
public:
	CUI_PopupWindow();
	virtual ~CUI_PopupWindow();

public:
	virtual bool beginUI() override;
	virtual void endUI() override;


private:
	ImGuiPopupFlags m_PopupFlags;

	ImVec2			m_vPopupPos;	// UI ��ġ
	bool			m_bModal;		// ��� or ��޸���



public:
	void SetModal(bool _Modal) { m_bModal = _Modal; }
	bool IsModal() { return m_bModal; }

	ImVec2 GetPopupPos() { return m_vPopupPos; }
	void SetPopupPos(ImVec2 _vPos) { m_vPopupPos = _vPos; }



};

