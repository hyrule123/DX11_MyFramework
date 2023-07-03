#include "pch.h"
#include "CEngine.h"

#include "CDevice.h"
#include "CPathMgr.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CResMgr.h"
#include "CLevelMgr.h"
#include "CRenderMgr.h"
#include "CEventMgr.h"
#include "CCollisionMgr.h"
#include "CRandMgr.h"
#include "CUserClassMgr.h"

#include "S_H_Struct.hlsli"


CEngine::CEngine()
	: m_hWnd(nullptr)
	, m_ClearColor(0.5f, 0.5f, 0.5f, 1.f)
	, m_bWinSizeChanged(true)
{
}

CEngine::~CEngine()
{
}

int CEngine::init(HWND _hWnd, UINT _uWidth, UINT _uHeight, UINT _uWndWidth, UINT _uWndHeight)
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
	if (FAILED(CDevice::GetInst()->init(m_hWnd, _uWidth, _uHeight)))
	{
		MessageBoxW(nullptr, L"Device 초기화 실패", NULL, MB_OK);
		return E_FAIL;
	}

	// Manager 생성 및 초기화
	CPathMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CTimeMgr::GetInst()->init();

	CResMgr::GetInst()->init();

	CLevelMgr::GetInst()->init();		
	CRenderMgr::GetInst()->init();
	CEventMgr::GetInst();
	CCollisionMgr::GetInst();

	CUserClassMgr::GetInst();

	return S_OK;
}

void CEngine::progress()
{
	tick();

	render();

	CEventMgr::GetInst()->tick();
}

void CEngine::tick()
{
	// Manager Tick
	CTimeMgr::GetInst()->tick();
	CKeyMgr::GetInst()->tick();	
	CLevelMgr::GetInst()->tick();
	CCollisionMgr::GetInst()->tick();
	CRenderMgr::GetInst()->tick();
}

void CEngine::render()
{
	CTimeMgr::GetInst()->render();

	// 출력 타겟 설정
	CDevice::GetInst()->SetRenderTarget();

	// 렌더링 시작
	CDevice::GetInst()->ClearTarget(m_ClearColor);

	//렌더링. 렌더타겟 플립은 여기서 진행하지 않음. EditorObjMgr의 렌더링 과정도 끝난 이후 진행
	CRenderMgr::GetInst()->render();
}

void CEngine::SetWndSize(UINT32 _uWidth, UINT32 _uHeight)
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

void CEngine::cleartick()
{
	m_bWinSizeChanged = false;

	CResMgr::GetInst()->cleartick();
	CEventMgr::GetInst()->cleartick();
}

void CEngine::present()
{
	CDevice::GetInst()->present();
}
