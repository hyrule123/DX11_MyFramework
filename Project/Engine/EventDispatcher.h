#pragma once

#include "define.h"

//단순 Static 멤버함수를 저장하는 용도의 클래스
//이벤트를 생성해서 전달해주는 역할을 한다.
class CGameObject;
class EventDispatcher
{
public:
	static void SpawnGameObject(CGameObject* _pNewObject, Vec3 _vWorldPos, int _LayerIdx);
	static void DestroyGameObj(CGameObject* _pObject);
	static void AddChildGameObj(CGameObject* _pParent, CGameObject* _pChild);
	static void RemoveComponent(CGameObject* _pObject, eCOMPONENT_TYPE _eType);
};

