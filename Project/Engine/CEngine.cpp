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

CEngine::CEngine()
	: m_hWnd(nullptr)
	, m_ClearColor(0.5f, 0.5f, 0.5f, 1.f)
{
}

CEngine::~CEngine()
{

}

int CEngine::init(HWND _hWnd, UINT _iWidth, UINT _iHeight)
{
	// ���� ������ �ڵ�
	m_hWnd = _hWnd;
	m_vResolution = Vec2((float)_iWidth, (float)_iHeight);
	g_GlobalVal.vResolution = m_vResolution;

	// �ػ󵵿� �´� �۾����� ũ�� ����
	RECT rt = { 0, 0, (int)_iWidth, (int)_iHeight};
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hWnd, nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0);
	ShowWindow(m_hWnd, true);


	// Device �ʱ�ȭ
	if (FAILED(CDevice::GetInst()->init(m_hWnd, _iWidth, _iHeight)))
	{
		MessageBoxW(nullptr, L"Device �ʱ�ȭ ����", NULL, MB_OK);
		return E_FAIL;
	}


	// Manager �ʱ�ȭ
	CPathMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CTimeMgr::GetInst()->init();
	CResMgr::GetInst()->init();

	CLevelMgr::GetInst()->init();		
	CRenderMgr::GetInst()->init();
	CEventMgr::GetInst();
	CCollisionMgr::GetInst();

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

	// ��� Ÿ�� ����
	CDevice::GetInst()->SetRenderTarget();

	// ������ ����
	CDevice::GetInst()->ClearTarget(m_ClearColor);

	//������. ����Ÿ�� �ø��� ���⼭ �������� ����. EditorObjMgr�� ������ ������ ���� ���� ����
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
