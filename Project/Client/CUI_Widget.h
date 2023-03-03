#pragma once

#include "CUI_BasicWindow.h"

//IMGUI�� �ּ� ���� ��Ҹ� Ŭ����ȭ �Ѱ�.
//�ݵ�� �θ� UI�� �־�� �Ѵ�. �ܵ����δ� ����� �Ұ���.
//�޺��ڽ� ���� ������ �������� �Ǿ��ִ� ������ ��� ������ �� Ŭ������ ��ӹ޾� ������ �� ���

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

