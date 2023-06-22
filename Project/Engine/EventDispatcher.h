#pragma once

#include "define.h"

#include "ptr.h"
#include "CPrefab.h"

//단순 Static 멤버함수를 저장하는 용도의 클래스
//이벤트를 생성해서 전달해주는 역할을 한다.
class CGameObject;
class EventDispatcher
{
public:
	static void SpawnGameObject(CGameObject* _pNewObject, const Vec3& _vWorldPos, int _LayerIdx = -1);

	//프리팹(리소스)를 넣어서 게임오브젝트를 스폰시킬 경우 해당 게임오브젝트의 주소를 리턴해줌(추가적인 설정을 할수있게)	
	static CGameObject* SpawnPrefab2D(Ptr<CPrefab> _Prefab, const Vec2& _vWorldPosXY);
	static CGameObject* SpawnPrefab(Ptr<CPrefab> _Prefab, const Vec3& _vWorldPos);
	static CGameObject* SpawnPrefab(Ptr<CPrefab> _Prefab);

	static void DestroyGameObj(CGameObject* _pObject);
	static void AddChildGameObj(CGameObject* _pParent, CGameObject* _pChild);
	static void RemoveComponent(CGameObject* _pObject, eCOMPONENT_TYPE _eType);
};


