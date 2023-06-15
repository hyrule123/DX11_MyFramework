#pragma once

#include "CSingleton.h"


class CEngine
	:public CSingleton<CEngine>
{
	SINGLETON(CEngine);

public:
	int init(HWND _hWnd, UINT _uWidth, UINT _uHeight, UINT _uWndWidth, UINT _uWndHeight);
	void progress();
	void present();

	//한 사이클이 끝나기 전까지 지속적으로 유지되어야 하는 값들을 초기화
	void cleartick();

private:
	void tick();
	void render();

private:
	HWND	m_hWnd;
public:
	HWND GetMainWnd() { return m_hWnd; }

private:
	bool m_bWinSizeChanged;
	
public:
	void SetWndSize(UINT32 _uWidth, UINT32 _uHeight);
	bool IsWinSizeChanged() const { return m_bWinSizeChanged; }	

private:
	Vec4	m_ClearColor;
};
