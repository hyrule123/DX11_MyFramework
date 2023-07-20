#include "pch.h"
#include "cEventMgr.h"

#include "cLevelMgr.h"
#include "cLevel.h"
#include "cGameObject.h"
#include "IComponent.h"



cEventMgr::cEventMgr()
	: m_bLevelModified()
{

}

cEventMgr::~cEventMgr()
{
	//이벤트 매니저에 이벤트가 남아있을 경우 여기에만 포인터가 남아 있으면 메모리 릭이 발생한다
	if (false == m_vecEvent.empty())
	{
		size_t size = m_vecEvent.size();
		for (size_t i = 0; i < size; ++i)
		{
			//오브젝트 생성 = 여기서만 주소를 들고 있으므로 메모리릭 발생 위험성 있음
			if (eEVENT_TYPE::SPAWN_OBJECT == m_vecEvent[i].Type)
			{
				cGameObject* pObj = reinterpret_cast<cGameObject*>(m_vecEvent[i].lParam);
				SAFE_DELETE(pObj);
			}
		}
	}
}

void cEventMgr::SpawnNewGameObj(const tGameEvent& _event)
{
	cGameObject* Obj = reinterpret_cast<cGameObject*>(_event.lParam);

	//오브젝트가 nullptr이거나 이미 게임 안에서 생성되었을 경우 return
	if (nullptr == Obj)
		return;

	cLevelMgr::GetInst()->GetCurLevel()->AddNewGameObj(Obj);

	m_bLevelModified = true;
}

void cEventMgr::DestroyGameObj(const tGameEvent& _event)
{
	cGameObject* _pObj = reinterpret_cast<cGameObject*>(_event.lParam);

	//이미 삭제 대기 상태에 들어가 있는 경우 추가로 등록하지 않음.
	//중복 삭제 방지(댕글링 포인터 위험)
	if (true == _pObj->IsDestroyed())
		return;

	_pObj->DestroyRecursive();

	//만약 부모 오브젝트는 삭제 대상이 아닐 경우 부모로부터 연결 해제
	cGameObject* pParent = _pObj->GetParent();
	if (pParent && false == pParent->IsDestroyed())
	{
		pParent->RemoveChild(_pObj);
	}
}



void cEventMgr::AddChildGameObj(const tGameEvent& _event)
{
	cGameObject* pParent = reinterpret_cast<cGameObject*>(_event.lParam);
	cGameObject* pChild = reinterpret_cast<cGameObject*>(_event.wParam);
	
	pParent->AddChildGameObj(pChild);
}

void cEventMgr::RemoveComponent(const tGameEvent& _event)
{
	//lParam = cGameObject*
	//wParam = eCOMPONENT_TYPE

	cGameObject* pObj = reinterpret_cast<cGameObject*>(_event.lParam);
	if (nullptr == pObj)
		return;

	eCOMPONENT_TYPE comType = (eCOMPONENT_TYPE)_event.wParam;

	IComponent* pCom = pObj->GetComponent(comType);
	if (nullptr == pCom)
		return;

	//여기선 Disable만 해줌으로써 모든 관계를 정리하도록 해준다.
	pCom->SetDisable(true);
	pCom->CleanUp();

	//나머지는 LazyEvent에서 처리.
	m_vecLazyEvent.push_back(_event);
}

void cEventMgr::RemoveComponentLazy(const tGameEvent& _event)
{
	cGameObject* pObj = reinterpret_cast<cGameObject*>(_event.lParam);
	if (nullptr == pObj)
		return;

	eCOMPONENT_TYPE comType = (eCOMPONENT_TYPE)_event.wParam;
	IComponent* pCom = pObj->GetComponent(comType);
	if (nullptr == pCom)
		return;

	//여기선 Disable만 해줌으로써 모든 관계를 정리하도록 해준다.
	pObj->RemoveComponent(comType);
}

void cEventMgr::ProcessEvent()
{
	size_t size = m_vecEvent.size();
	for (size_t i = 0; i < size; ++i)
	{
		switch (m_vecEvent[i].Type)
		{
		case eEVENT_TYPE::SPAWN_OBJECT:
			SpawnNewGameObj(m_vecEvent[i]);
			break;
		case eEVENT_TYPE::DELETE_OBJECT:
			DestroyGameObj(m_vecEvent[i]);
			break;
		case eEVENT_TYPE::ADD_CHILD:
			AddChildGameObj(m_vecEvent[i]);
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

void cEventMgr::ProcessLazyEvent()
{
	//bDestroy 상태인 게임오브젝트를 cLevel에서 제거시킨다.(댕글링 포인터 제거)
	cLevelMgr::GetInst()->GetCurLevel()->RemoveDestroyed();

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


void cEventMgr::Tick()
{
	ProcessLazyEvent();
	ProcessEvent();
}
