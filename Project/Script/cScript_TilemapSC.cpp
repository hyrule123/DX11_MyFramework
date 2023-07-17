#include "pch.h"

#include "cScript_TilemapSC.h"

//유닛 로드 및 언로드 용
#include <Engine/CResMgr.h>
#include <Engine/strKey_Default.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/EventDispatcher.h>
#include <Engine/cGameObject.h>
#include <Engine/CDevice.h>
#include <Engine/CConstBuffer.h>
#include <Engine/cRenderMgr.h>
#include <Engine/cCom_Transform.h>
#include <Engine/CTilemapComplete.h>

#include "define_SC.h"
#include "CCS_SCMapLoader.h"
#include "S_H_Tilemap_SC.hlsli"

#include "cScript_Mineral.h"
#include "strKey_Script.h"
#include "strKey_CShader.h"
#include "strKey_GShader.h"
#include <Engine/UserClassMgr.h>

#include <Engine/RandGen.h>
#include <Engine/cCom_Camera.h>

//디버그 출력 확인용
using namespace SC_Map;

cScript_TilemapSC::cScript_TilemapSC(const string_view _strKey)
	: IScript(_strKey)
	, m_pMapData()
	, m_bMapLoaded()
	, m_bUnitLoaded()
	, m_eDebugMode()
{
}

cScript_TilemapSC::~cScript_TilemapSC()
{
}


void cScript_TilemapSC::init()
{
	CTilemap* pTilemap = Tilemap();
	if (nullptr == pTilemap)
	{
		GetOwner()->AddComponent(new CTilemapComplete);
		Tilemap();
	}

	//메쉬는 부모 클래스에서 설정했음.
	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(strKey_RES_DEFAULT::MATERIAL::TILEMAP_COMPLETE);

	Ptr<CGraphicsShader> pShader = CResMgr::GetInst()->Load<CGraphicsShader>(strKey_GShader::Tilemap_SC);
	assert(nullptr != pShader);

	pTilemap->SetMaterial(pMtrl);
	pTilemap->GetCurMaterial()->SetShader(pShader);
}

void cScript_TilemapSC::tick()
{
	if (m_bMapLoaded)
	{
		//디버그 모드 설정
		int DebugMode = DEBUGMODE_TILE_NONE;
		if (cRenderMgr::GetInst()->IsEditorCamMode())
			DebugMode = (int)m_eDebugMode;
		GetOwner()->SetMtrlScalarParam(MTRL_SCALAR_INT_DEBUGMODE, &DebugMode);


		//맵 로딩 후 유닛이 로드되지 않았을 경우 유닛 로드
		if (false == m_bUnitLoaded)
		{
			m_bUnitLoaded = true;

			LoadUnit();
		}
	}
}

//인스턴싱 X
void cScript_TilemapSC::BindData()
{
	if (false == m_bMapLoaded)
		return;

	//이번에 출력될 카메라 인덱스를 자신의 Scalar Data에 등록
	cGameObject* pOwner = GetOwner();

	//맵 정보 바인딩
	g_SBufferShareData.iData0 = (int)m_pMapData->eTileSet;
	g_SBufferShareData.iData1 = (int)m_pMapData->uNumMegatileX;
	g_SBufferShareData.iData2 = (int)m_pMapData->uNumMegatileY;

	//맵 데이터 바인딩
	m_pMapData->pSBuffer_MXTM->BindBufferSRV();
	m_pMapData->pSBufferRW_Megatile->BindBufferSRV();
	m_pMapData->pSBufferRW_Minitile->BindBufferSRV();
}

void cScript_TilemapSC::UnBind()
{
	m_pMapData->pSBuffer_MXTM->BindBufferSRV();
	m_pMapData->pSBufferRW_Megatile->BindBufferSRV();
	m_pMapData->pSBufferRW_Minitile->BindBufferSRV();
}

bool cScript_TilemapSC::LoadMap(const string_view _strMapName)
{
	if (true == m_bMapLoaded && _strMapName == m_pMapData->strMapName)
		return true;

	if (m_bUnitLoaded)
	{
		UnloadUnit();
	}
	m_bUnitLoaded = false;

	Ptr<CCS_SCMapLoader> pLoader = UserClassMgr::GetNewCS(strKey_CShader::CCS_SCMapLoader);
	

	//기존 리소스가 있을 시 제거 요청
	if (m_pMapData && nullptr != m_pMapData->pMapTex)
	{
		m_pMapData->pMapTex = nullptr;
		Tilemap()->
		GetCurMaterial()->SetTexParam(eMTRLDATA_PARAM_TEX::_0, nullptr);
		CResMgr::GetInst()->DeleteRes<CTexture>(m_pMapData->strMapName);
	}

	//데이터 초기화
	m_pMapData = pLoader->LoadMap(_strMapName);

	if (m_pMapData && m_pMapData->bMapLoaded)
		m_bMapLoaded = true;
	else
		m_bMapLoaded = false;

	if (true == m_bMapLoaded)
	{
		Tilemap()->GetCurMaterial()->SetTexParam(eMTRLDATA_PARAM_TEX::_0, m_pMapData->pMapTex);

		cCom_Transform& pTF = Transform();

		g_GlobalVal.fMapSizeX = (float)m_pMapData->uNumMegatileX * 32.f;
		g_GlobalVal.fMapSizeY = (float)m_pMapData->uNumMegatileY * 32.f;
		
		pTF.SetSize(Vec3(g_GlobalVal.fMapSizeX, g_GlobalVal.fMapSizeY, 1.f));

		//#define MTRL_SCALAR_VEC2_MAPSIZE	  MTRLDATA_PARAM_SCALAR(VEC2, 2)
		{
			Vec2 MapSize = Vec2(g_GlobalVal.fMapSizeX, g_GlobalVal.fMapSizeY);
			GetOwner()->SetMtrlScalarParam(MTRL_SCALAR_VEC2_MAPSIZE, &MapSize);
		}

		UINT MapSizeX = m_pMapData->uNumMegatileX;
		UINT MapSizeY = m_pMapData->uNumMegatileY;

		//#define MTRL_SCALAR_VEC2_MEGATILESIZE MTRLDATA_PARAM_SCALAR(VEC2, 0)
		{
			Vec2 MapSize = Vec2(MapSizeX, MapSizeY);
			GetOwner()->SetMtrlScalarParam(MTRL_SCALAR_VEC2_MEGATILESIZE, &MapSize);
		}

		//#define MTRL_SCALAR_VEC2_MINITILESIZE MTRLDATA_PARAM_SCALAR(VEC2, 1)
		{
			//메가타일에는 각 미니타일 4개씩이 들어있다.
			MapSizeX *= 4u;
			MapSizeY *= 4u;
			Vec2 MapSize = Vec2(MapSizeX, MapSizeY);
			GetOwner()->SetMtrlScalarParam(MTRL_SCALAR_VEC2_MINITILESIZE, &MapSize);
		}
		



		//스타크래프트 맵의 좌표는 Left Top이 원점이므로
		//맵의 좌측 상단을 원점으로 맞춰준다(2사분면에 집어넣어준다)
		//y축만 음수로 뒤집으면 되니까 좌표 맞추기가 편해짐
		Vec2 MapPos = Vec2(g_GlobalVal.fMapSizeX, -g_GlobalVal.fMapSizeY);
		MapPos /= 2.f;
		
		pTF.SetRelativePosXY(MapPos);
	}

	return m_bMapLoaded;
}


void cScript_TilemapSC::LoadUnit()
{
	using namespace SC_Map;
	using namespace SC;

	size_t size = m_pMapData->vecUnitData.size();
	for (size_t i = 0; i < size; ++i)
	{
		const tUnitData& unit = m_pMapData->vecUnitData[i];

		Ptr<CPrefab> UnitPrefab = CResMgr::GetInst()->Load<CPrefab>(SC::GetUnitName((SC::eUNIT_ID)unit.ID));

		//유닛 생성. Y좌표계는 반전해줘야 함
		cGameObject* SpawnedObj = nullptr;
		if (nullptr != UnitPrefab)
		{
			SpawnedObj = EventDispatcher::SpawnPrefab2D(UnitPrefab, Vec2((float)unit.PosX, -(float)unit.PosY));
		}


		//유닛에 따라서 추가적인 작업을 해야 할 경우 여기서 해줄것
		switch (unit.ID)
		{
		case (UINT16)SC::eUNIT_ID::MINERAL_FIELD_TYPE_1:
		{
			if (nullptr == SpawnedObj)
				break;
			//미네랄 스프라이트 설정
			int MineralType = 0;
			SpawnedObj->SetMtrlScalarParam(MTRL_SCALAR_MINERAL_TEXINDEX, &MineralType);

			//미네랄 남은 자원량 설정
			cScript_Mineral* pScriptMineral = static_cast<cScript_Mineral*>(SpawnedObj->ScriptHolder()->FindScript(strKey_Script::cScript_Mineral));
			pScriptMineral->SetMineralLeft((UINT)unit.Resources);

			break;

		}

		case (UINT16)SC::eUNIT_ID::MINERAL_FIELD_TYPE_2:
		{
			if (nullptr == SpawnedObj)
				break;
			//미네랄 스프라이트 설정
			int MineralType = 1;
			SpawnedObj->SetMtrlScalarParam(MTRL_SCALAR_MINERAL_TEXINDEX, &MineralType);

			//미네랄 남은 자원량 설정
			cScript_Mineral* pScriptMineral = static_cast<cScript_Mineral*>(SpawnedObj->ScriptHolder()->FindScript(strKey_Script::cScript_Mineral));
			pScriptMineral->SetMineralLeft((UINT)unit.Resources);

			break;
		}

		case (UINT16)SC::eUNIT_ID::MINERAL_FIELD_TYPE_3:
		{
			if (nullptr == SpawnedObj)
				break;
			//미네랄 스프라이트 설정
			int MineralType = 2;
			SpawnedObj->SetMtrlScalarParam(MTRL_SCALAR_MINERAL_TEXINDEX, &MineralType);

			//미네랄 남은 자원량 설정
			cScript_Mineral* pScriptMineral = static_cast<cScript_Mineral*>(SpawnedObj->ScriptHolder()->FindScript(strKey_Script::cScript_Mineral));
			pScriptMineral->SetMineralLeft((UINT)unit.Resources);

			break;
		}

		case (UINT16)SC::eUNIT_ID::VESPENE_GEYSER:
		{
			if (nullptr == SpawnedObj)
				break;
			float CurTileSet = (float)m_pMapData->eTileSet;
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


	StartLocation();
}

void cScript_TilemapSC::UnloadUnit()
{
	m_vecStartLocation.clear();

	{
		CLayer& ResourceLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(SC::LAYER_INFO::Resource);
		const vector<cGameObject*>& vecObj = ResourceLayer.GetvecObj();
		size_t size = vecObj.size();
		for (size_t i = 0; i < size; ++i)
		{
			EventDispatcher::DestroyGameObj(vecObj[i]);
		}
	}

	{
		CLayer& ResourceLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(SC::LAYER_INFO::GroundUnitMain);
		const vector<cGameObject*>& vecObj = ResourceLayer.GetvecObj();
		size_t size = vecObj.size();
		for (size_t i = 0; i < size; ++i)
		{
			EventDispatcher::DestroyGameObj(vecObj[i]);
		}
	}

}

void cScript_TilemapSC::StartLocation()
{
	if (m_vecStartLocation.empty())
		return;

	//스타팅 포인트 중 랜덤한 곳에 커맨드 생성
	int StartPos = (int)m_vecStartLocation.size();
	StartPos = RandGen::GetInst()->GetRand(0, StartPos - 1);
	{
		Ptr<CPrefab> Command = CResMgr::GetInst()->Load<CPrefab>(SC::GetUnitName(SC::eUNIT_ID::TERRAN_COMMAND_CENTER));
		assert(nullptr != Command);

		EventDispatcher::SpawnPrefab2D(Command, m_vecStartLocation[StartPos]);

		cCom_Camera* pMainCam = cRenderMgr::GetInst()->GetCamera(eCAMERA_INDEX::MAIN);
		assert(pMainCam);
		pMainCam->GetOwner()->Transform().SetRelativePosXY(m_vecStartLocation[StartPos]);

		Ptr<CPrefab> Marine = CResMgr::GetInst()->Load<CPrefab>(SC::GetUnitName(SC::eUNIT_ID::TERRAN_MARINE));

		assert(nullptr != Marine);

		EventDispatcher::SpawnPrefab2D(Marine, m_vecStartLocation[StartPos] + Vec2(100.f, 100.f));
	}
}
