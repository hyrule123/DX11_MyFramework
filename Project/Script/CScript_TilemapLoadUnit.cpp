#include "pch.h"
#include "CScript_TilemapLoadUnit.h"

#include "define_SC.h"
#include <Engine/CTileMap_SC.h>
#include <Engine/EventDispatcher.h>
#include <Engine/CResMgr.h>

CScript_TilemapLoadUnit::CScript_TilemapLoadUnit(const string& _strKey)
	: CScript(_strKey)
{
}


CScript_TilemapLoadUnit::~CScript_TilemapLoadUnit()
{
}

void CScript_TilemapLoadUnit::init()
{
	static_cast<CTilemap_SC*>(Tilemap())->SetFunc_LoadUnit(std::bind(&CScript_TilemapLoadUnit::LoadUnit, this, std::placeholders::_1));
}

void CScript_TilemapLoadUnit::LoadUnit(const SC_Map::tMapData& _data)
{
	using namespace SC_Map;
	using namespace SC;
	size_t size = _data.vecUnitData.size();
	for (size_t i = 0; i < size; ++i)
	{
		const tUnitData& unit = _data.vecUnitData[i];
		if ((UINT16)SC::eUNIT_ID::MINERAL_FIELD_TYPE_1 == unit.ID)
		{
			Ptr<CPrefab> Mineral = CResMgr::GetInst()->Load<CPrefab>(SC::GetUnitName(eUNIT_ID::MINERAL_FIELD_TYPE_1));
			
			assert(nullptr != Mineral);

			EventDispatcher::SpawnGameObject(Mineral->Instantiate(), Vec3((float)unit.PosX, -(float)unit.PosY, 10.f), LAYER_INFO::GroundUnitMain);
		}
	}
}
