#include "pch.h"
#include "LevelMgr.h"

#include "Level.h"
#include "Layer.h"

#include "GameObject.h"
#include "components.h"
#include "MeshRenderer.h"

//Scripts

#include "ResMgr.h"


//Camera Register
#include "RenderMgr.h"

//충돌 레이어 등록 및 체크용
#include "CollisionMgr.h"

//타일맵
#include "Tilemap.h"

//컴퓨트쉐이더 테스트
#include "strKey_Default.h"

#include "TimeMgr.h"

#include "Collider2D_Point.h"


LevelMgr::LevelMgr()
	: m_pCurLevel(nullptr)
{
}

LevelMgr::~LevelMgr()
{
	if (nullptr != m_pCurLevel)
		delete m_pCurLevel;
}



void LevelMgr::SetCurLevel(Level* _pLevel)
{
	assert(nullptr);
}

void LevelMgr::AddNewGameObj(GameObject* _pObj)
{
	assert(m_pCurLevel || nullptr == _pObj);
	m_pCurLevel->AddNewGameObj(_pObj);
}

GameObject* LevelMgr::FindObjectByName(const string_view _Name)
{
	return m_pCurLevel->FindObjectByName(_Name);
}

void LevelMgr::FindObjectALLByName(const string_view _Name, vector<GameObject*>& _vecObj)
{
	m_pCurLevel->FindObjectALLByName(_Name, _vecObj);
}

void LevelMgr::init()
{
	m_pCurLevel = new Level;
}

void LevelMgr::tick()
{
	m_pCurLevel->tick();
	m_pCurLevel->finaltick();
}
