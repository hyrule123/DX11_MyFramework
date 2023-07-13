#include "pch.h"
#include "cEngine.h"

#include "Device.h"
#include "PathMgr.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "ResMgr.h"
#include "LevelMgr.h"
#include "RenderMgr.h"
#include "EventMgr.h"
#include "CollisionMgr.h"
#include "RandGen.h"
#include "UserClassMgr.h"

#include "S_H_Struct.hlsli"


cEngine::cEngine()
	: m_hWnd(nullptr)
	, m_ClearColor(0.5f, 0.5f, 0.5f, 1.f)
	, m_bWinSizeChanged(true)
{
}

cEngine::~cEngine()
{
}

int cEngine::init(HWND _hWnd, UINT _uWidth, UINT _uHeight, UINT _uWndWidth, UINT _uWndHeight)
{
	// 메인 윈도우 핸들
	m_hWnd = _hWnd;

	g_GlobalVal.u2WinSize = UINT32_2(_uWndWidth, _uWndHeight);
	g_GlobalVal.v2WinSize = Vec2(_uWndWidth, _uWndHeight);

	//창을 모니터 정중앙에 표시하기 위한 Left Top 좌표 구하기
	int MonitorSizeX = GetSystemMetrics(SM_CXSCREEN);
	int MonitorSizeY = GetSystemMetrics(SM_CYSCREEN);

	int scrMidX = MonitorSizeX / 2 - (int)(_uWndWidth / 2u);
	int scrMidY = MonitorSizeY / 2 - (int)(_uWndHeight / 2u);

	if (scrMidX + (int)_uWndWidth > MonitorSizeX)
		scrMidX = 10;
	if (scrMidY + (int)_uWndHeight > MonitorSizeY)
		scrMidY = 10;

	//위치 지정
	SetWindowPos(m_hWnd, nullptr, scrMidX, scrMidY, 0, 0, SWP_NOSIZE);

	SetWndSize(_uWndWidth, _uWndHeight);
	
	ShowWindow(m_hWnd, true);
	
	// Device 초기화
	if (FAILED(Device::GetInst()->init(m_hWnd, _uWidth, _uHeight)))
	{
		MessageBoxW(nullptr, L"Device 초기화 실패", NULL, MB_OK);
		return E_FAIL;
	}

	// Manager 생성 및 초기화
	PathMgr::GetInst()->init();
	KeyMgr::GetInst()->init();
	TimeMgr::GetInst()->init();

	ResMgr::GetInst()->init();

	LevelMgr::GetInst()->init();		
	RenderMgr::GetInst()->init();
	EventMgr::GetInst();
	CollisionMgr::GetInst();

	return S_OK;
}

void cEngine::progress()
{
	tick();

	render();

	EventMgr::GetInst()->tick();
}

void cEngine::tick()
{
	// Manager Tick
	TimeMgr::GetInst()->tick();
	KeyMgr::GetInst()->tick();	
	LevelMgr::GetInst()->tick();
	CollisionMgr::GetInst()->tick();
	RenderMgr::GetInst()->tick();
}

void cEngine::render()
{
	TimeMgr::GetInst()->render();

	// 출력 타겟 설정
	Device::GetInst()->SetRenderTarget();

	// 렌더링 시작
	Device::GetInst()->ClearTarget(m_ClearColor);

	//렌더링. 렌더타겟 플립은 여기서 진행하지 않음. EditorObjMgr의 렌더링 과정도 끝난 이후 진행
	RenderMgr::GetInst()->render();
}

void cEngine::SetWndSize(UINT32 _uWidth, UINT32 _uHeight)
{
	g_GlobalVal.u2WinSize.x = _uWidth; 
	g_GlobalVal.u2WinSize.y = _uHeight; 
	g_GlobalVal.v2WinSize = Vec2(g_GlobalVal.u2WinSize.x, g_GlobalVal.u2WinSize.y);
	m_bWinSizeChanged = true;
	
	//클라이언트 영역과 윈도우 영역의 차이를 구해서 정확한 창 크기를 설정(해상도가 조금이라도 차이나면 문제 발생함)
	RECT rc, rcClient;
	GetWindowRect(m_hWnd, &rc);
	GetClientRect(m_hWnd, &rcClient);

	// calculate size of non-client area
	int xExtra = rc.right - rc.left - rcClient.right;
	int yExtra = rc.bottom - rc.top - rcClient.bottom;

	// now resize based on desired client size
	SetWindowPos(m_hWnd, 0, 0u, 0u, _uWidth + xExtra, _uHeight + yExtra, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void cEngine::cleartick()
{
	m_bWinSizeChanged = false;

	ResMgr::GetInst()->cleartick();
	EventMgr::GetInst()->cleartick();
}

void cEngine::present()
{
	Device::GetInst()->present();
}
