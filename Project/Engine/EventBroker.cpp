#include "pch.h"
#include "EventBroker.h"

#include "CEventMgr.h"
#include "CGameObject.h"
#include "CTransform.h"

void EventBroker::SpawnGameObject(CGameObject* _pNewObject, Vec3 _vWorldPos, int _LayerIdx)
{
	_pNewObject->Transform()->SetRelativePos(_vWorldPos);

	tEvent evn = {};
	evn.Type = eEVENT_TYPE::CREATE_OBJECT;
	evn.lParam = reinterpret_cast<DWORD_PTR>(_pNewObject);
	evn.rParam = static_cast<DWORD_PTR>(_LayerIdx);

	CEventMgr::GetInst()->AddEvent(evn);
}

void EventBroker::DestroyObject(CGameObject* _pObject)
{
	tEvent evn = {};
	evn.Type = eEVENT_TYPE::DELETE_OBJECT;
	evn.lParam = reinterpret_cast<DWORD_PTR>(_pObject);
	evn.rParam = 0;

	CEventMgr::GetInst()->AddEvent(evn);
}

void EventBroker::AddChildObj(CGameObject* _pParent, CGameObject* _pChild)
{
	tEvent evn = {};
	evn.Type = eEVENT_TYPE::ADD_CHILD;
	evn.lParam = reinterpret_cast<DWORD_PTR>(_pParent);
	evn.rParam = reinterpret_cast<DWORD_PTR>(_pChild);

	CEventMgr::GetInst()->AddEvent(evn);
}
