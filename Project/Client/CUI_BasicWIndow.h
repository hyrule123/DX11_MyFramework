#pragma once

#include "CUI.h"

class CUI_Widget;

//UI_Interface ������ �׸���� ���״�� ���������Լ��� �������̽��� �����ϴ� ������ �����.

//1. ������ : UI�� �����Ҵ� ����
//2. Init : UI�� ���� ����
//3. tick : UI���� ó���ؾ� �ϴ� ���� �۾� ����. â�� ������ �ʾƵ� �۵��ؾ��� �۾����� ���⼭
//4. finaltick : �������̽� �����. beginOOO ~ endOOO ���� ���̾ƿ� �������̽� ����(UI_Interface ����)
//5. render_update : ������ �ν��Ͻ��� ������ Ŭ�������� ������. UI�� ��ü���� ���̾ƿ� ����.

class CUI_BasicWindow
	: public CUI
{
private:
	CUI_BasicWindow() = delete;
public:
	CUI_BasicWindow(const string& _Name);
	virtual ~CUI_BasicWindow();

public:
	virtual void init() {}
	virtual void tick() {}

	//�������ؼ� ���� UI�� �´� begin�Լ��� ȣ��. bool ���� ��ȯ�Ǹ�, true�� ��ȯ�Ǿ��� ������ endUI()�� ȣ���.
	//�׷��Ƿ� ������ endUI()�� ȣ���ϰ� ���� ��쿡�� true�� �������� ��ȯ������ �� 
	//bool���� ��ȯ ���ο� ���� endUI() ȣ�� ���ΰ� �޶����� IMGUI �Լ�(ex.ComboBox)�� ��쿡��
	//return ������ �ش� �Լ��� ȣ���ϸ� �ȴ�.
	virtual bool beginUI();

	//���⿡�� Ŭ�������� Ư���ϰ� ȣ���ؾ��� ���̾ƿ��� ���� ��� �������ؼ� ����ϸ� ��.
	virtual void render_update() {}

	//�������ؼ� ���� UI�� �´� end�Լ��� ȣ��
	virtual void endUI();

	//������ ���̾ƿ� ��ġ �۾��� ���⼭ ����. �Ʒ��� render_update�� �ݵ�� ȣ������� �Ѵ�.
	//finaltick()�� ������ ���� �ƿ� �ٸ� ������� �����ؾ��� ���� �ֱ� ������ virtual ���·� �д�.


private:
	ImGuiWindowFlags m_WindowFlag;

	//Child
	bool			m_bBorder;
	ImVec2			m_vSize;		// UI ũ��

public:
	void SetWindowFlags(ImGuiWindowFlags _uWindowFlag) { m_WindowFlag = _uWindowFlag; }
	ImGuiWindowFlags GetWindowFlag() const { return m_WindowFlag; }

	void SetBorder(bool _b) { m_bBorder = _b; }

	void SetSize(float _width, float _height) { m_vSize = ImVec2(_width, _height); }
	ImVec2 GetSize() const { return m_vSize; }
	float GetWidth() const { return m_vSize.x; }
	float GetHeight() const { return m_vSize.y; }
	bool SizeSet() const { return (0.f == m_vSize.x) && (0.f == m_vSize.y); }
};