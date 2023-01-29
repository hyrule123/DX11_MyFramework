#pragma once

#include "CUI.h"

//�޺��ڽ� ���� ������ �������� �Ǿ��ִ� ������ ��� ������ �� Ŭ������ ��ӹ޾� ������ �� ���

enum class eWIDGET_TYPE : DWORD
{
	COMBO_BOX
};

class CWidget
	: public CUI
{
private:
	CWidget() = delete;
public:
	CWidget(const string& _Name, eWIDGET_TYPE _Type);
	virtual ~CWidget();

private:
	const eWIDGET_TYPE m_Type;

	bool m_bLeftLabel;
	float m_fLeftLabelWidth;


public:
	void SetLeftLabel(bool _b) { m_bLeftLabel = _b; }
	bool GetLeftLabel() const { return m_bLeftLabel; }

	void SetLeftLabelWidth(float _Width) { m_fLeftLabelWidth = _Width; }
	float GetLeftLabelWidth() const { return m_fLeftLabelWidth; }

public:
	virtual void finaltick() override;
	virtual int render_update() = 0;
};

