#include "pch.h"
#include "cLevelMgr.h"

#include "cLevel.h"
#include "cLayer.h"

#include "cGameObject.h"
#include "components.h"
#include "cCom_Renderer_Basic.h"

//Scripts

#include "cResMgr.h"


//cCom_Camera Register
#include "cRenderMgr.h"

//충돌 레이어 등록 및 체크용
#include "cCollisionMgr.h"

//타일맵
#include "ITilemapBase.h"

//컴퓨트쉐이더 테스트
#include "strKey_Default.h"

#include "cTimeMgr.h"

#include "cCom_Coll2D_Point.h"


cLevelMgr::cLevelMgr()
	: m_pCurLevel(nullptr)
{
}

cLevelMgr::~cLevelMgr()
{
	if (nullptr != m_pCurLevel)
		delete m_pCurLevel;
}



void cLevelMgr::SetCurLevel(cLevel* _pLevel)
{
	assert(nullptr);
}

void cLevelMgr::AddNewGameObj(cGameObject* _pObj)
{
	assert(m_pCurLevel || nullptr == _pObj);
	m_pCurLevel->AddNewGameObj(_pObj);
}

cGameObject* cLevelMgr::FindObjectByName(const string_view _Name)
{
	return m_pCurLevel->FindObjectByName(_Name);
}

void cLevelMgr::FindObjectALLByName(const string_view _Name, vector<cGameObject*>& _vecObj)
{
	m_pCurLevel->FindObjectALLByName(_Name, _vecObj);
}

void cLevelMgr::init()
{
	m_pCurLevel = new cLevel;
}

void cLevelMgr::tick()
{
	m_pCurLevel->tick();
	m_pCurLevel->finaltick();
}
