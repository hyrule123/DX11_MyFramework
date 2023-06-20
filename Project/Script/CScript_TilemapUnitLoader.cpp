#include "pch.h"
#include "CScript_TilemapUnitLoader.h"

#include "define_SC.h"
#include <Engine/CTileMap_SC.h>
#include <Engine/EventDispatcher.h>
#include <Engine/CResMgr.h>

#include <Engine/S_H_SCUnitGround.hlsli>

CScript_TilemapUnitLoader::CScript_TilemapUnitLoader(const string& _strKey)
	: CScript(_strKey)
{
}


CScript_TilemapUnitLoader::~CScript_TilemapUnitLoader()
{
}

void CScript_TilemapUnitLoader::init()
{
	CTilemap_SC* pTilemapSC = dynamic_cast<CTilemap_SC*>(Tilemap());
	assert(pTilemapSC);

	pTilemapSC->SetFunc_LoadUnit(std::bind(&CScript_TilemapUnitLoader::LoadUnit, this, std::placeholders::_1));
}

void CScript_TilemapUnitLoader::LoadUnit(const SC_Map::tMapData& _data)
{
	using namespace SC_Map;
	using namespace SC;
	UINT16 Mineral1 = (UINT16)SC::eUNIT_ID::MINERAL_FIELD_TYPE_1;
	UINT16 Mineral2 = (UINT16)SC::eUNIT_ID::MINERAL_FIELD_TYPE_2;
	UINT16 Mineral3 = (UINT16)SC::eUNIT_ID::MINERAL_FIELD_TYPE_3;

	size_t size = _data.vecUnitData.size();
	for (size_t i = 0; i < size; ++i)
	{
		const tUnitData& unit = _data.vecUnitData[i];
		
		//미네랄 1 ~ 3 은 같은 프리팹을 사용(내부 변수만 바꿔주면 됨)
		if (Mineral1 <= unit.ID && unit.ID <= Mineral3)
		{
			Ptr<CPrefab> MPrefab = CResMgr::GetInst()->Load<CPrefab>(SC::GetUnitName(eUNIT_ID::MINERAL_FIELD_TYPE_1));
			
			assert(nullptr != MPrefab);

			CGameObject* pMineral = MPrefab->Instantiate();
			EventDispatcher::SpawnGameObject(pMineral, Vec3((float)unit.PosX, -(float)unit.PosY, 10.f), LAYER_INFO::GroundUnitMain);

			int MineralType = 0;

			if (Mineral1 == unit.ID)
				MineralType = (int)eMINERAL_ATLAS_TYPE::_1;
			else if (Mineral2 == unit.ID)
				MineralType = (int)eMINERAL_ATLAS_TYPE::_2;
			else if (Mineral3 == unit.ID)
				MineralType = (int)eMINERAL_ATLAS_TYPE::_3;

			pMineral->SetMtrlScalarParam(MTRL_SCALAR_MINERAL_TEXINDEX, &MineralType);
		}
	}
}
