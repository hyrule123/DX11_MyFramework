#include "pch.h"
#include "CScript_Vespene.h"

#include <Engine/CTransform.h>




CScript_Vespene::CScript_Vespene(const string& _strKey)
	:CScript_SCEntity(_strKey)
{
}

CScript_Vespene::~CScript_Vespene()
{
}

void CScript_Vespene::init()
{
	CScript_SCEntity::init();

	//자식 오브젝트(연기) 들의 위치 지정
	enum ePos
	{
		Left,
		Mid,
		Right
	};
	const vector<CGameObject*>& vecObj = GetOwner()->GetvecChilds();
	size_t size = vecObj.size();
	for (size_t i = 0; i < size; ++i)
	{
		assert(3ui64 == size);
		Vec2 Pos;
		switch (i)
		{
		case Left:
			Pos = Vec2(-45.f, 15.f);
			break;
		case Mid:
			Pos = Vec2(10.f, 30.f);
			break;
		case Right:
			Pos = Vec2(30.f, 5.f);
			break;
		default:
			break;
		}
		vecObj[i]->Transform().SetRelativePosXY(Pos);
	}
	
}
