#include "pch.h"

#include "define.h"
#include "CKeyMgr.h"
#include "CEngine.h"



int g_arrVK[(UINT)eKEY::END]
=
{
	 VK_UP,
	 VK_DOWN,
	 VK_LEFT,
	 VK_RIGHT,

	 VK_SPACE,
	 VK_RETURN,
	 VK_ESCAPE,
	 VK_LMENU,
	 VK_LCONTROL,
	 VK_LSHIFT,
	 VK_TAB,

	 VK_LBUTTON,
	 VK_RBUTTON,

	 'Q',
	 'W',
	 'E',
	 'R',
	 'T',
	 'Y',
	 'U',
	 'I',
	 'O',
	 'P',

	 'A',
	 'S',
	 'D',
	 'F',

	 'Z',
	 'X',
	 'C',
	 'V',

	 '0',
	 '1',
	 '2',
	 '3',
	 '4',
	 '5',
	 '6',
	 '7',
	 '8',
	 '9',

};





CKeyMgr::CKeyMgr()
{

}

CKeyMgr::~CKeyMgr()
{

}


void CKeyMgr::init()
{
	for (int i = 0; i < (int)eKEY::END; ++i)
	{
		m_vecKey.push_back(tKeyInfo{ (eKEY)i , eKEY_STATE::NONE });
	}
}

void CKeyMgr::tick()
{
	if (GetFocus())
	{
		size_t size = m_vecKey.size();
		for (size_t i = 0; i < size; ++i)
		{
			if (GetAsyncKeyState(g_arrVK[(UINT)m_vecKey[i].key]) & 0x8000)
			{
				// 이전에는 눌리지 않았다.
				if (false == m_vecKey[i].bPrev)
				{
					m_vecKey[i].state = eKEY_STATE::DOWN;
					m_vecKey[i].bPrev = true;
				}
				else
				{
					// 지금도 눌려있고, 이전 프레임에서도 눌려있었다.
					m_vecKey[i].state = eKEY_STATE::PRESSED;
				}
			}
			else
			{
				// 눌려있지 않다.
				if (false == m_vecKey[i].bPrev)
				{
					m_vecKey[i].state = eKEY_STATE::NONE;
				}
				else
				{
					m_vecKey[i].state = eKEY_STATE::RELEASE;
					m_vecKey[i].bPrev = false;
				}
			}
		}

		// Mouse 위치 갱신. 현재 위치, 이전 위치, 마우스 방향 3가지 계산
		POINT ptMousePos = {};
		GetCursorPos(&ptMousePos);		
		ScreenToClient(CEngine::GetInst()->GetMainWnd(), &ptMousePos);
		m_vPrevMousePos = m_vMouseLocalPos;
		m_vMouseLocalPos = Vec2((float)ptMousePos.x, fabsf(g_GlobalVal.vResolution.y - (float)ptMousePos.y));
		m_vMouseDir = m_vMouseLocalPos - m_vPrevMousePos;
		//윈도우 창 좌표계와 DX 좌표계의 y축은 서로 반대이므로
		//m_vMouseDir.y *= -1.f;
	}

	// Window 가 focus 상태가 아니다
	else
	{
		
		size_t size = m_vecKey.size();
		for (size_t i = 0; i < size; ++i)
		{
			if (eKEY_STATE::DOWN == m_vecKey[i].state || eKEY_STATE::PRESSED == m_vecKey[i].state)
			{
				m_vecKey[i].state = eKEY_STATE::RELEASE;
			}

			else if (eKEY_STATE::RELEASE == m_vecKey[i].state)
			{
				m_vecKey[i].state = eKEY_STATE::NONE;
			}
		}
	}	
}
