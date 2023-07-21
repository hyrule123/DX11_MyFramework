#pragma once

#include "define.h"

#include "Ptr.h"
#include "cPrefab.h"

//단순 Static 멤버함수를 저장하는 용도의 클래스
//이벤트를 생성해서 전달해주는 역할을 한다.
class cGameObject;
class EventDispatcher
{
public:
	static void SpawnGameObject(cGameObject* _pNewObject, const Vec3& _vWorldPos, int _LayerIdx = -1);

	//프리팹(리소스)를 넣어서 게임오브젝트를 스폰시킬 경우 해당 게임오브젝트의 주소를 리턴해줌(추가적인 설정을 할수있게)	
	static cGameObject* SpawnPrefab2D(Ptr<cPrefab> _Prefab, const Vec2& _vWorldPosXY);
	static cGameObject* SpawnPrefab(Ptr<cPrefab> _Prefab, const Vec3& _vWorldPos);
	static cGameObject* SpawnPrefab(Ptr<cPrefab> _Prefab);

	static void DestroyGameObj(cGameObject* _pObject);
	static void AddChildGameObj(cGameObject* _pParent, cGameObject* _pChild);
	static void RemoveComponent(cGameObject* _pObject, eCOMPONENT_TYPE _eType);
};


