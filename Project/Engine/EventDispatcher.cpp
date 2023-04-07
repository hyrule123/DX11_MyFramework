#include "pch.h"
#include "EventDispatcher.h"
#include "CEventMgr.h"

#include "CLevelMgr.h"
#include "CGameObject.h"
#include "CTransform.h"

void EventDispatcher::SpawnGameObject(CGameObject* _pNewObject, Vec3 _vWorldPos, int _LayerIdx)
{
	assert(0 <= _LayerIdx && _LayerIdx < MAX_LAYER);

	_pNewObject->Transform()->SetRelativePos(_vWorldPos);
	_pNewObject->SetLayer(_LayerIdx);

	tGameEvent evn = {};
	evn.Type = eEVENT_TYPE::CREATE_OBJECT;
	evn.lParam = reinterpret_cast<DWORD_PTR>(_pNewObject);

	CEventMgr::GetInst()->AddEvent(evn);
}

void EventDispatcher::DestroyGameObj(CGameObject* _pObject)
{
	tGameEvent evn = {};
	evn.Type = eEVENT_TYPE::DELETE_OBJECT;
	evn.lParam = reinterpret_cast<DWORD_PTR>(_pObject);
	evn.rParam = 0;

	CEventMgr::GetInst()->AddEvent(evn);
}

void EventDispatcher::AddChildGameObj(CGameObject* _pParent, CGameObject* _pChild)
{

	tGameEvent evn = {};
	evn.Type = eEVENT_TYPE::ADD_CHILD;
	evn.lParam = reinterpret_cast<DWORD_PTR>(_pParent);
	evn.rParam = reinterpret_cast<DWORD_PTR>(_pChild);

	CEventMgr::GetInst()->AddEvent(evn);
}

void EventDispatcher::RemoveComponent(CGameObject* _pObject, eCOMPONENT_TYPE _eType)
{
	//오브젝트가 없거나 오브젝트 안의 컴포넌트가 없을 경우 return
	if (nullptr == _pObject || nullptr == _pObject->GetComponent(_eType))
		return;

	tGameEvent evn = {};

	evn.Type = eEVENT_TYPE::REMOVE_COMPONENT;
	evn.lParam = reinterpret_cast<DWORD_PTR>(_pObject);
	evn.rParam = (DWORD_PTR)(_eType);

	CEventMgr::GetInst()->AddEvent(evn);
}
