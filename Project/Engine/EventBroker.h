#pragma once

#include "define.h"

//단순 Static 멤버함수를 저장하는 용도의 클래스
class CGameObject;
class EventBroker
{
public:
	static void SpawnGameObject(CGameObject* _pNewObject, Vec3 _vWorldPos, int _LayerIdx);
	static void DestroyObject(CGameObject* _pObject);
	static void AddChildObj(CGameObject* _pParent, CGameObject* _pChild);
};

