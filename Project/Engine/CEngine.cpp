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
#include "CScriptMgr.h"


CEngine::CEngine()
	: m_hWnd(nullptr)
	, m_ClearColor(0.5f, 0.5f, 0.5f, 1.f)
{
}

CEngine::~CEngine()
{
}

int CEngine::init(HWND _hWnd, UINT _uWidth, UINT _uHeight, UINT _uWndWidth, UINT _uWndHeight)
{
	// 메인 윈도우 핸들
	m_hWnd = _hWnd;

	g_GlobalVal.u2Res = UINT32_2(_uWidth, _uHeight);
	g_GlobalVal.u2ResWnd = UINT32_2(_uWndWidth, _uWndHeight);

	// 해상도에 맞는 작업영역 크기 조정
	RECT rt = { 0, 0, (int)_uWndWidth, (int)_uWndHeight};
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);

	int scrMidX = GetSystemMetrics(SM_CXSCREEN) / 2 - (int)(g_GlobalVal.u2ResWnd.x / 2u);
	int scrMidY = GetSystemMetrics(SM_CYSCREEN) / 2 - (int)(g_GlobalVal.u2ResWnd.y / 2u);


	SetWindowPos(m_hWnd, nullptr, scrMidX, scrMidY, (int)_uWndWidth, (int)_uWndHeight, 0);
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

	CScriptMgr::GetInst();

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
	RECT WndRect = {};
	GetWindowRect(m_hWnd, &WndRect);

	g_GlobalVal.u2ResWnd.x = (UINT32)(WndRect.right - WndRect.left);
	g_GlobalVal.u2ResWnd.y = (UINT32)(WndRect.top - WndRect.bottom);

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

void CEngine::cleartick()
{
	CResMgr::GetInst()->cleartick();
	CEventMgr::GetInst()->cleartick();
}

void CEngine::present()
{
	CDevice::GetInst()->present();
}
