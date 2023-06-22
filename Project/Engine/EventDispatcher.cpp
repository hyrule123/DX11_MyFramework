#include "pch.h"
#include "EventDispatcher.h"
#include "CEventMgr.h"

#include "CLevelMgr.h"
#include "CGameObject.h"
#include "CTransform.h"

#include "CPrefab.h"

//깊이 프리셋 값이 설정되어 있을 경우 WorldPos의 Z값은 무시됨.
void EventDispatcher::SpawnGameObject(CGameObject* _pNewObject, const Vec3& _vWorldPos, int _LayerIdx)
{
	_pNewObject->Transform().SetRelativePos(_vWorldPos);

	if (0 <= _LayerIdx)
		_pNewObject->SetLayer(_LayerIdx);
	else
	{
		assert(0 <= _pNewObject->GetLayer() && _pNewObject->GetLayer() < MAX_LAYER);
	}


	tGameEvent evn = {};
	evn.Type = eEVENT_TYPE::SPAWN_OBJECT;
	evn.lParam = reinterpret_cast<DWORD_PTR>(_pNewObject);

	CEventMgr::GetInst()->AddEvent(evn);
}

CGameObject* EventDispatcher::SpawnPrefab(Ptr<CPrefab> _Prefab, const Vec2& _vWorldPosXY)
{
	assert(nullptr != _Prefab && _Prefab->IsAvailable());
	CGameObject* pObj = _Prefab->Instantiate();
	assert(pObj);
	assert(0 <= pObj->GetLayer() && pObj->GetLayer() < MAX_LAYER);

	pObj->Transform().SetRelativePosXY(_vWorldPosXY);

	tGameEvent evn = {};
	evn.Type = eEVENT_TYPE::SPAWN_OBJECT;
	evn.lParam = reinterpret_cast<DWORD_PTR>(pObj);

	CEventMgr::GetInst()->AddEvent(evn);

	return pObj;
}

CGameObject* EventDispatcher::SpawnPrefab(Ptr<CPrefab> _Prefab)
{
	assert(nullptr != _Prefab && _Prefab->IsAvailable());
	CGameObject* pObj = _Prefab->Instantiate();
	assert(pObj);
	assert(0 <= pObj->GetLayer() && pObj->GetLayer() < MAX_LAYER);

	tGameEvent evn = {};
	evn.Type = eEVENT_TYPE::SPAWN_OBJECT;
	evn.lParam = reinterpret_cast<DWORD_PTR>(pObj);

	CEventMgr::GetInst()->AddEvent(evn);

	return pObj;
}



void EventDispatcher::DestroyGameObj(CGameObject* _pObject)
{
	tGameEvent evn = {};
	evn.Type = eEVENT_TYPE::DELETE_OBJECT;
	evn.lParam = reinterpret_cast<DWORD_PTR>(_pObject);
	evn.wParam = 0;

	CEventMgr::GetInst()->AddEvent(evn);
}

void EventDispatcher::AddChildGameObj(CGameObject* _pParent, CGameObject* _pChild)
{

	tGameEvent evn = {};
	evn.Type = eEVENT_TYPE::ADD_CHILD;
	evn.lParam = reinterpret_cast<DWORD_PTR>(_pParent);
	evn.wParam = reinterpret_cast<DWORD_PTR>(_pChild);

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
	evn.wParam = (DWORD_PTR)(_eType);

	CEventMgr::GetInst()->AddEvent(evn);
}
