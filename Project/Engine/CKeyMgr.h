#pragma once

#include "define.h"

#include "CSingleton.h"

enum class eKEY
{
	UP,		// VK_UP
	DOWN,
	LEFT,
	RIGHT,

	SPACE,
	ENTER,
	ESC,
	LALT,
	LCTRL,
	LSHIFT,
	TAB,

	LBTN,
	RBTN,
	
	Q,
	W,
	E,
	R,
	T,
	Y,
	U,
	I,
	O,
	P,

	A,
	S,
	D,
	F,

	Z,
	X,
	C,
	V,

	_0,
	_1,
	_2,
	_3,
	_4,
	_5,
	_6,
	_7,
	_8,
	_9,
	
	END,
};




struct tKeyInfo
{
	eKEY			key;
	eKEY_STATE	state;
	bool		bPrev;
};


class CKeyMgr
	: public CSingleton<CKeyMgr>
{
	SINGLETON(CKeyMgr);
private:
	vector<tKeyInfo>	m_vecKey;
	Vec2				m_vMouseLocalPos;
	Vec2				m_vPrevMousePos;
	Vec2				m_vMouseDir;

public:
	void init();
	void tick();


public:
	eKEY_STATE GetKeyState(eKEY _key) { return m_vecKey[(UINT)_key].state; }
	Vec2 GetMouseLocalPos() { return m_vMouseLocalPos; }
	Vec2 GetMouseDir() { return m_vMouseDir; }
};

