#include "pch.h"
#include "CEventMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CGameObject.h"


CEventMgr::CEventMgr()
	: m_bLevelModified()
{

}

CEventMgr::~CEventMgr()
{

}

void CEventMgr::CreateObject(const tEvent& _event)
{
	//lParam = CGameObject Pointer
	//rParam = Layer Index
	CGameObject* Obj = reinterpret_cast<CGameObject*>(_event.lParam);

	//오브젝트가 nullptr이거나 이미 게임 안에서 생성되었을 경우 return
	if (nullptr == Obj || Obj->IsInitialized())
		return;

	CLevelMgr::GetInst()->GetCurLevel()->AddGameObject(Obj, (int)_event.rParam);

	m_bLevelModified = true;
}

void CEventMgr::DestroyObject(const tEvent& _event)
{
	CGameObject* _pObj = reinterpret_cast<CGameObject*>(_event.lParam);

	//이미 삭제 대기 상태에 들어가 있는 경우 추가로 등록하지 않음.
	//중복 삭제 방지(댕글링 포인터 위험)
	if (true == _pObj->IsDestroyed())
		return;

	_pObj->DestroyForEventMgr();
	m_vecReserveDestroy.push_back(_pObj);

	m_bLevelModified = true;
}

void CEventMgr::AddChildObj(const tEvent& _event)
{
	CGameObject* pParent = reinterpret_cast<CGameObject*>(_event.lParam);
	CGameObject* pChild = reinterpret_cast<CGameObject*>(_event.rParam);
	
	pParent->AddChildObj(pChild);
}

void CEventMgr::tick()
{
	//bDestroy 상태인 게임오브젝트를 Level에서 제거
	CLevelMgr::GetInst()->GetCurLevel()->RemoveDestroyed();

	//이벤트 큐가 진행되기 전에 이전 프레임에서 등록된 오브젝트를 제거
	//이러면 지난 프레임에서 제거하도록 등록되었던 오브젝트들이 전부 지워지게 됨.
	size_t size = m_vecReserveDestroy.size();
	for (size_t i = 0; i < size; ++i)
	{
		delete m_vecReserveDestroy[i];
	}
	m_vecReserveDestroy.clear();


	size = m_vecEvent.size();
	for (size_t i = 0; i < size; ++i)
	{
		switch (m_vecEvent[i].Type)
		{
		case eEVENT_TYPE::CREATE_OBJECT:
			CreateObject(m_vecEvent[i]);
			break;
		case eEVENT_TYPE::DELETE_OBJECT:
			DestroyObject(m_vecEvent[i]);
			break;
		case eEVENT_TYPE::ADD_CHILD:
			AddChildObj(m_vecEvent[i]);
			break;
		case eEVENT_TYPE::DELETE_RESOURCE:
			break;
		case eEVENT_TYPE::LEVEL_CHANGE:
			break;
		default:
			break;
		}
	}
	m_vecEvent.clear();
}
