#include "pch.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CEventMgr.h"

void SpawnGameObject(CGameObject* _pNewObject, Vec3 _vWorldPos, int _LayerIdx)
{
	_pNewObject->Transform()->SetRelativePos(_vWorldPos);

	tEvent evn = {};
	evn.Type = eEVENT_TYPE::CREATE_OBJECT;
	evn.wParam = (DWORD_PTR)_pNewObject;
	evn.lParam = (DWORD_PTR)_LayerIdx;

	CEventMgr::GetInst()->AddEvent(evn);
}

void DestroyObject(CGameObject* _pObject)
{

}
