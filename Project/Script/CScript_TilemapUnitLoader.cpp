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

#include "strKey_Script.h"
#include "CScript_Mineral.h"

CScript_TilemapUnitLoader::CScript_TilemapUnitLoader(const string_view _strKey)
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

		Ptr<CPrefab> UnitPrefab = CResMgr::GetInst()->Load<CPrefab>(SC::GetUnitName((SC::eUNIT_ID)unit.ID));

		//아직 해당 유닛의 프리팹이 만들어지지 않았을 경우 continue
		if (nullptr == UnitPrefab)
			continue;

		//유닛 생성. Y좌표계는 반전해줘야 함
		CGameObject* SpawnedObj = EventDispatcher::SpawnPrefab2D(UnitPrefab, Vec2((float)unit.PosX, -(float)unit.PosY));

		//유닛에 따라서 추가적인 작업을 해야 할 경우 여기서 해줄것
		switch (unit.ID)
		{
		case (UINT16)SC::eUNIT_ID::MINERAL_FIELD_TYPE_1:
		{
			//미네랄 스프라이트 설정
			int MineralType = 0;
			SpawnedObj->SetMtrlScalarParam(MTRL_SCALAR_MINERAL_TEXINDEX, &MineralType);

			//미네랄 남은 자원량 설정
			CScript_Mineral* pScriptMineral = static_cast<CScript_Mineral*>(SpawnedObj->ScriptHolder()->FindScript(strKey_SCRIPT::MINERAL));
			pScriptMineral->SetMineralLeft((UINT)unit.Resources);
			
			break;
		}

		case (UINT16)SC::eUNIT_ID::MINERAL_FIELD_TYPE_2:
		{
			//미네랄 스프라이트 설정
			int MineralType = 0;
			SpawnedObj->SetMtrlScalarParam(MTRL_SCALAR_MINERAL_TEXINDEX, &MineralType);

			//미네랄 남은 자원량 설정
			CScript_Mineral* pScriptMineral = static_cast<CScript_Mineral*>(SpawnedObj->ScriptHolder()->FindScript(strKey_SCRIPT::MINERAL));
			pScriptMineral->SetMineralLeft((UINT)unit.Resources);

			break;
		}

		case (UINT16)SC::eUNIT_ID::MINERAL_FIELD_TYPE_3:
		{
			//미네랄 스프라이트 설정
			int MineralType = 0;
			SpawnedObj->SetMtrlScalarParam(MTRL_SCALAR_MINERAL_TEXINDEX, &MineralType);

			//미네랄 남은 자원량 설정
			CScript_Mineral* pScriptMineral = static_cast<CScript_Mineral*>(SpawnedObj->ScriptHolder()->FindScript(strKey_SCRIPT::MINERAL));
			pScriptMineral->SetMineralLeft((UINT)unit.Resources);

			break;
		}

		case (UINT16)SC::eUNIT_ID::VESPENE_GEYSER:
		{
			float CurTileSet = (float)_pMapData->eTileSet;
			SpawnedObj->SetMtrlScalarParam(MTRL_SCALAR_FLOAT_VESPINE_SPRITE, &CurTileSet);
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

