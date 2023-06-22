#include "pch.h"
#include "CScript_TilemapUnitLoader.h"

#include "define_SC.h"
#include <Engine/CTileMap_SC.h>
#include <Engine/EventDispatcher.h>
#include <Engine/CResMgr.h>

//Unload
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>

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

	std::function<void(const shared_ptr<SC_Map::tMapData>)> pFuncLoad = std::bind(&CScript_TilemapUnitLoader::LoadUnit, this, std::placeholders::_1);
	std::function<void(void)> pFuncUnload = std::bind(&CScript_TilemapUnitLoader::UnloadUnit, this);

	pTilemapSC->SetFunc_LoadUnit(pFuncLoad, pFuncUnload);
}

void CScript_TilemapUnitLoader::LoadUnit(const shared_ptr<SC_Map::tMapData> _pMapData)
{
	using namespace SC_Map;
	using namespace SC;


	size_t size = _pMapData->vecUnitData.size();
	for (size_t i = 0; i < size; ++i)
	{
		const tUnitData& unit = _pMapData->vecUnitData[i];

		int MineralType = -1;
		

		switch (unit.ID)
		{
		case (UINT16)SC::eUNIT_ID::MINERAL_FIELD_TYPE_1:
			MineralType = (int)eMINERAL_ATLAS_TYPE::_1;
			break;
		case (UINT16)SC::eUNIT_ID::MINERAL_FIELD_TYPE_2:
			MineralType = (int)eMINERAL_ATLAS_TYPE::_2;
			break;
		case (UINT16)SC::eUNIT_ID::MINERAL_FIELD_TYPE_3:
			MineralType = (int)eMINERAL_ATLAS_TYPE::_3;
			break;
		case (UINT16)SC::eUNIT_ID::VESPENE_GEYSER:
		{
			Ptr<CPrefab> VPrefab = CResMgr::GetInst()->Load<CPrefab>(SC::GetUnitName(eUNIT_ID::VESPENE_GEYSER));

			assert(nullptr != VPrefab);

			CGameObject* pVespene = EventDispatcher::SpawnPrefab(VPrefab, Vec2((float)unit.PosX, -(float)unit.PosY));

			float CurTileSet = (float)_pMapData->MapInfo.eTileSet;
			pVespene->SetMtrlScalarParam(MTRL_SCALAR_FLOAT_VESPINE_SPRITE, &CurTileSet);
		}
		break;

		case (UINT16)SC::eUNIT_ID::START_LOCATION:
		{
			m_vecStartLocation.push_back(Vec2((float)unit.PosX, -(float)unit.PosY));
		}


		break;


		default:
			break;
		}


		if (-1 != MineralType)
		{
			Ptr<CPrefab> MPrefab = CResMgr::GetInst()->Load<CPrefab>(SC::GetUnitName(eUNIT_ID::MINERAL_FIELD_TYPE_1));

			assert(nullptr != MPrefab);

			CGameObject* pMineral = MPrefab->Instantiate();

			EventDispatcher::SpawnGameObject(pMineral, Vec3((float)unit.PosX, -(float)unit.PosY, 10.f));

			pMineral->SetMtrlScalarParam(MTRL_SCALAR_MINERAL_TEXINDEX, &MineralType);
		}
	}
}

void CScript_TilemapUnitLoader::UnloadUnit()
{
	m_vecStartLocation.clear();

	CLayer& ResourceLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(SC::LAYER_INFO::Resource);
	const vector<CGameObject*>& vecObj = ResourceLayer.GetvecObj();
	size_t size = vecObj.size();
	for (size_t i = 0; i < size; ++i)
	{
		EventDispatcher::DestroyGameObj(vecObj[i]);
	}
}

