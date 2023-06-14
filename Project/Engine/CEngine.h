#pragma once

#include "CSingleton.h"

class CEngine
	:public CSingleton<CEngine>
{
	SINGLETON(CEngine);

private:
	HWND	m_hWnd;

	//해상도는 Global Variable에 저장
	bool	m_bWinResChanged;
public:
	bool IsResChanged() const { return m_bWinResChanged; }

private:
	Vec4	m_ClearColor;
	

public:
	HWND GetMainWnd() { return m_hWnd ; }

public:
	int init(HWND _hWnd, UINT _uWidth, UINT _uHeight, UINT _uWndWidth, UINT _uWndHeight);
	void progress();
	void present();

	//한 사이클이 끝나기 전까지 지속적으로 유지되어야 하는 값들을 초기화
	void cleartick();

private:
	void tick();
	void render();
};
