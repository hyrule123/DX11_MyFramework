#include "pch.h"
#include "CLevelMgr.h"

#include "CLevel.h"
#include "CLayer.h"

#include "CGameObject.h"
#include "components.h"
#include "CMeshRender.h"

//Scripts

#include "CResMgr.h"


//Camera Register
#include "CRenderMgr.h"

//충돌 레이어 등록 및 체크용
#include "CCollisionMgr.h"

//타일맵
#include "CTilemap.h"

//컴퓨트쉐이더 테스트
#include "CCS_SetColor.h"

#include "CParticleSystem.h"

#include "strKeyDefault.h"

#include "CTimeMgr.h"

#include "CCollider2D_Point.h"


CLevelMgr::CLevelMgr()
	: m_pCurLevel(nullptr)
{
}

CLevelMgr::~CLevelMgr()
{
	if (nullptr != m_pCurLevel)
		delete m_pCurLevel;
}



void CLevelMgr::SetCurLevel(CLevel* _pLevel)
{
	assert(nullptr);
}

void CLevelMgr::AddNewGameObj(CGameObject* _pObj)
{
	assert(m_pCurLevel || nullptr == _pObj);
	m_pCurLevel->AddNewGameObj(_pObj);
}

CGameObject* CLevelMgr::FindObjectByName(const string& _Name)
{
	return m_pCurLevel->FindObjectByName(_Name);
}

void CLevelMgr::FindObjectALLByName(const string& _Name, vector<CGameObject*>& _vecObj)
{
	m_pCurLevel->FindObjectALLByName(_Name, _vecObj);
}

void CLevelMgr::init()
{
	m_pCurLevel = new CLevel;
}

void CLevelMgr::tick()
{
	m_pCurLevel->tick();
	m_pCurLevel->finaltick();
}
