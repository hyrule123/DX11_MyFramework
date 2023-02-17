#pragma once

class CEngine
	: public CSingleton<CEngine>
{
public:
	CEngine();
	~CEngine();

private:
	HWND	m_hWnd;

	// ������ �ػ�
	Vec2	m_vResolution;

	Vec4	m_ClearColor;
	

public:
	Vec2 GetWindowResolution() { return m_vResolution; }
	HWND GetMainWnd() { return m_hWnd ; }

public:
	int init(HWND _hWnd, UINT _iWidth, UINT _iHeight);
	void progress();
	void present();

	//�� ����Ŭ�� ������ ������ ���������� �����Ǿ�� �ϴ� ������ �ʱ�ȭ
	void cleartick();

private:
	void tick();
	void render();



};
