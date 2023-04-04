#include "pch.h"
#include "CEventMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CGameObject.h"
#include "CComponent.h"

DEFINITION_SINGLETON(CEventMgr);

CEventMgr::CEventMgr()
	: m_bLevelModified()
{

}

CEventMgr::~CEventMgr()
{

}

void CEventMgr::CreateObject(const tEvent& _event)
{

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
}



void CEventMgr::AddChildObj(const tEvent& _event)
{
	CGameObject* pParent = reinterpret_cast<CGameObject*>(_event.lParam);
	CGameObject* pChild = reinterpret_cast<CGameObject*>(_event.rParam);
	
	pParent->AddChildObj(pChild);
}

void CEventMgr::RemoveComponent(const tEvent& _event)
{
	//lParam = GameObject*
	//rParam = eCOMPONENT_TYPE

	CGameObject* pObj = reinterpret_cast<CGameObject*>(_event.lParam);
	if (nullptr == pObj)
		return;

	eCOMPONENT_TYPE comType = (eCOMPONENT_TYPE)_event.rParam;

	CComponent* pCom = pObj->GetComponent(comType);
	if (nullptr == pCom)
		return;

	//여기선 Disable만 해줌으로써 모든 관계를 정리하도록 해준다.
	pCom->SetDisable(true);
	pCom->cleanup();

	//나머지는 LazyEvent에서 처리.
	m_vecLazyEvent.push_back(_event);
}

void CEventMgr::RemoveComponentLazy(const tEvent& _event)
{
	CGameObject* pObj = reinterpret_cast<CGameObject*>(_event.lParam);
	if (nullptr == pObj)
		return;

	eCOMPONENT_TYPE comType = (eCOMPONENT_TYPE)_event.rParam;
	CComponent* pCom = pObj->GetComponent(comType);
	if (nullptr == pCom)
		return;

	//여기선 Disable만 해줌으로써 모든 관계를 정리하도록 해준다.
	pObj->RemoveComponent(comType);
}

void CEventMgr::ProcessLazyEvent()
{
	//bDestroy 상태인 게임오브젝트를 Level에서 제거시킨다.(댕글링 포인터 제거)
	CLevelMgr::GetInst()->GetCurLevel()->RemoveDestroyed();

	size_t size = m_vecLazyEvent.size();
	for (size_t i = 0; i < size; ++i)
	{
		switch (m_vecLazyEvent[i].Type)
		{
		case eEVENT_TYPE::REMOVE_COMPONENT:
			RemoveComponentLazy(m_vecLazyEvent[i]);
			break;

		default:
			break;
		}
	}
	m_vecLazyEvent.clear();
}

void CEventMgr::ProcessEvent()
{
	size_t size = m_vecEvent.size();
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

		case eEVENT_TYPE::REMOVE_COMPONENT:
			RemoveComponent(m_vecEvent[i]);
			break;

		default:
			break;
		}
	}
	m_vecEvent.clear();
}

void CEventMgr::tick()
{
	ProcessLazyEvent();
	ProcessEvent();
}
