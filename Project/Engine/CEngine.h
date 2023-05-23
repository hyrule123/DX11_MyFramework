#pragma once

#include "CSingleton.h"

class CEngine
	:public CSingleton<CEngine>
{
	SINGLETON(CEngine);

private:
	HWND	m_hWnd;

	// 윈도우 해상도
	Vec2	m_vResolution;

	Vec4	m_ClearColor;
	

public:
	Vec2 GetWindowResolution() { return m_vResolution; }
	HWND GetMainWnd() { return m_hWnd ; }

public:
	int init(HWND _hWnd, UINT _iWidth, UINT _iHeight);
	void progress();
	void present();

	//한 사이클이 끝나기 전까지 지속적으로 유지되어야 하는 값들을 초기화
	void cleartick();

private:
	void tick();
	void render();
};
