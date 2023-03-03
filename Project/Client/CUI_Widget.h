#pragma once

#include "CUI_BasicWindow.h"

//IMGUI의 최소 구성 요소를 클래스화 한것.
//반드시 부모 UI가 있어야 한다. 단독으로는 사용이 불가능.
//콤보박스 같이 복잡한 구성으로 되어있는 위젯의 경우 별도로 이 클래스를 상속받아 구현한 뒤 사용

enum class eWIDGET_TYPE : DWORD
{
	COMBO_BOX,
	IMAGE,
	TREE,
	MENU_ITEM,
	DRAG_N_DROP_SENDER,
	DRAN_N_DROP_RECEIVER
};

class CUI_Widget
	: public CUI_BasicWindow
{
private:
	CUI_Widget() = delete;
public:
	CUI_Widget(const string& _strName, eWIDGET_TYPE _Type);
	virtual ~CUI_Widget();

public:
	virtual bool beginUI() override;
	virtual void render_update() override;
	virtual void endUI() override;

private:
	const eWIDGET_TYPE m_Type;

	bool m_bLeftLabel;
	float m_fLeftLabelWidth;

	bool m_bSizeSet;


public:
	void SetLeftLabel(bool _b) { m_bLeftLabel = _b; }
	bool GetLeftLabel() const { return m_bLeftLabel; }

	void SetLeftLabelWidth(float _Width) { m_fLeftLabelWidth = _Width; }
	float GetLeftLabelWidth() const { return m_fLeftLabelWidth; }


};

